
#include "pch.h"

#include "OpenGLDynamicRHI.h"
#include "OpenGLResource.h"
#include "../Globals.h"

namespace
{

template<
	size_t TMaxFrameAllocation,
	size_t DefaultAlignment = 16,
	size_t MaxAlignmentBits = 8,
	size_t MaxOffsetBits = 32 - MaxAlignmentBits>
class VertexBufferPoolAllocator
{
public:
	static_assert(MaxOffsetBits > MaxAlignmentBits, "Offset has les bits than alignment!");

	static const size_t MaxFrameAllocation = TMaxFrameAllocation;
	static const size_t TotalSize = MaxFrameAllocation * (1U << 2);
	static const size_t MaxAlignment = 1 << MaxAlignmentBits;
	static const size_t MaxOffset = 1 << MaxOffsetBits;

	struct VertexBufferPoolAllocation {
		uint8_t* Head;
		uint32_t MemberOffset;
		uint32_t SizeWithoutPadding;
		uint32_t AlignmentPadding;
		int32_t FrameRetired; // used in clean-up to detect if the allocation is retired
	};

	void* Allocate(OpenGLVertexBuffer* Key, uint32_t MemberOffset, uint32_t Size, uint16_t Alignment = DefaultAlignment) {
		if (!CheckInit()) {
			return nullptr;
		}

		// alignment must be power of two
		if ((DefaultAlignment & (DefaultAlignment - 1)) != 0)
			return nullptr;

		return InternalAllocate(Key, MemberOffset, Size, Alignment);
	}

	bool Deallocate(OpenGLVertexBuffer* VertexBuffer) {
		if (!OpenGLSpecs::SupportsBufferStorage()) {
			return false;
		}

		auto it = mAllocationMap.find(VertexBuffer);
		if (it == mAllocationMap.end()) {
			return false;
		}

		VertexBufferPoolAllocation* PoolAlloc = (*it).second;
		mAllocationMap.erase(it);

		VertexBuffer->Bind();
		glCopyBufferSubData(GL_COPY_READ_BUFFER, VertexBuffer->GetGLResource(),
							(PoolAlloc->Head - PoolAlloc->AlignmentPadding) - mMemoryBlock,
							PoolAlloc->MemberOffset, PoolAlloc->SizeWithoutPadding);

		PoolAlloc->FrameRetired = gFrameNumber;
		return true;
	}

	void Poll() {
		// wait x frames after retiring
		uint32_t Tolerance = 3;
		if (gFrameNumber < Tolerance) {
			return;
		}

		size_t TotalRetiring = 0;
		size_t ToleratedFrameCounter = gFrameNumber - Tolerance;
		while (TotalRetiring < mAllocations.size()) {
			VertexBufferPoolAllocation* PoolAlloc = mAllocations[TotalRetiring];
			if (PoolAlloc->FrameRetired < 0 || (size_t)PoolAlloc->FrameRetired > ToleratedFrameCounter) {
				break; // don't allow gaps
			}

			mFreeSpace += PoolAlloc->SizeWithoutPadding + PoolAlloc->AlignmentPadding;
			delete PoolAlloc;
			TotalRetiring++;
		}

		mAllocations.erase(mAllocations.begin(), mAllocations.begin() + TotalRetiring);
		mAllocatedThisFrame = 0;
	}

private:
	void* InternalAllocate(OpenGLVertexBuffer* Key, uint32_t MemberOffset, uint32_t Size, uint16_t Alignment = 16, bool Recurr = false) {
		uintptr_t HeadPtr = *reinterpret_cast<const uintptr_t*>(&mMemoryBlock) + mOffset;

		// since we permit dynamic padding, there would be padding; mostly this
		// this will be 0, due to the fact
		uint16_t AlignmentSubOne = Alignment - 1;
		uint32_t AlignmentPadding = ((HeadPtr + AlignmentSubOne) & (~AlignmentSubOne)) - HeadPtr;

		uint32_t TotalRequiredSize = Size + AlignmentPadding;
		if (TotalRequiredSize > MaxFrameAllocation - mAllocatedThisFrame || TotalRequiredSize > mFreeSpace) {
			return nullptr;
		}

		// the allocation actually has size, but the back of the block is not enough
		// so acquire the memory left and try again at the beginning
		// constraint: no gaps allowed, and this is well-defined by the frame poll
		if (TotalRequiredSize > (TotalSize - mOffset)) {
			if (Recurr) {
				return nullptr;
			}

			uint32_t PoolLeftover = TotalSize - mOffset;

			VertexBufferPoolAllocation* PoolAlloc = new VertexBufferPoolAllocation;
			PoolAlloc->Head = 0;
			PoolAlloc->MemberOffset = 0;
			PoolAlloc->SizeWithoutPadding = PoolLeftover;
			PoolAlloc->AlignmentPadding = 0;
			PoolAlloc->FrameRetired = gFrameNumber;
			mAllocations.push_back(PoolAlloc);

			// retry
			return InternalAllocate(Key, MemberOffset, Size, Alignment, true);
		}

		// there is actually enough size at the head, acquire
		VertexBufferPoolAllocation* PoolAlloc = new VertexBufferPoolAllocation;
		PoolAlloc->Head = mMemoryBlock + mOffset;
		PoolAlloc->MemberOffset = MemberOffset;
		PoolAlloc->AlignmentPadding = AlignmentPadding;
		PoolAlloc->SizeWithoutPadding = Size;
		PoolAlloc->FrameRetired = -1;

		mAllocations.push_back(PoolAlloc);
		mAllocationMap.insert({ Key, PoolAlloc });

		mOffset += TotalRequiredSize;
		mFreeSpace -= TotalRequiredSize;
		mAllocatedThisFrame += TotalRequiredSize;

		return PoolAlloc->Head + PoolAlloc->AlignmentPadding;
	}

	bool CheckInit() {
		if (!OpenGLSpecs::SupportsBufferStorage()) {
			return false;
		}

		// generate a buffer once and map it to be persistent & coherent
		if (!mBuffer) {
			OpenGLSpecs::GenBuffers(1, &mBuffer);
			OpenGLSpecs::BindBuffer(GL_COPY_READ_BUFFER, mBuffer);
			GLbitfield Access =
				GL_MAP_WRITE_BIT | // to be used only for writing
				GL_MAP_PERSISTENT_BIT | // updated regularly
				GL_MAP_COHERENT_BIT; // the data will be available at gpu right away

			// obtain a buffer storage with same access as mapping
			OpenGLSpecs::BufferStorage(GL_COPY_READ_BUFFER, TotalSize, NULL, Access);
			mMemoryBlock = OpenGLSpecs::MapBufferRange(GL_COPY_READ_BUFFER, 0, TotalSize,
													   OpenGLSpecs::GLResourceLockMode::RLM_WriteOnlyPersistent);
			mFreeSpace = TotalSize;
		}

		return true;
	}

	uint8_t* mMemoryBlock = nullptr;

	GLuint mBuffer = 0;
	uint32_t mOffset = 0;
	uint32_t mFreeSpace = 0;
	uint32_t mAllocatedThisFrame = 0;
	std::vector<VertexBufferPoolAllocation*> mAllocations;
	std::map<OpenGLVertexBuffer*, VertexBufferPoolAllocation*> mAllocationMap;
};

static const size_t PoolBufferFrameMax = 1024 * 1024 * 4;
VertexBufferPoolAllocator<PoolBufferFrameMax, 16> VBPool;

}

void OpenGLDynamicRHICleanVertexBuffer() {
	VBPool.Poll();
}

RHIVertexBufferRef OpenGLDynamicRHI::RHICreateVertexBuffer(size_t VerticesCount, size_t VertexSize, RHIHardwareBufferUsage Usage)
{
	RHI_THREAD_BEGIN_COMMAND();
	RHIVertexBufferRef VertexBuffer = new OpenGLVertexBuffer(VerticesCount, VertexSize, Usage);
	// TODO(slavi): initialize with data, buffer, ...
	return VertexBuffer;
	RHI_THREAD_END_COMMAND_RETURN(RHIVertexBufferRef);
}

void* OpenGLDynamicRHI::RHILockVertexBuffer(RHIVertexBuffer* VertexBufferRHI, size_t MemberOffset, size_t Size, RHIResourceLockMode LockMode)
{
	RHI_THREAD_BEGIN_COMMAND();
	OpenGLVertexBuffer* VertexBuffer = StaticResourceCast(VertexBufferRHI);

	void* ZeroStrideBuffer = VertexBuffer->GetZeroStrideBuffer();
	if (ZeroStrideBuffer &&  MemberOffset + Size <= VertexBuffer->GetSize()) {
		return (void*)((uint8_t*)ZeroStrideBuffer + MemberOffset);
	}

	// if the buffer is used for writing, approach AZDO
	if (VertexBuffer->IsDynamicBuffer() && LockMode == RHIResourceLockMode::RLM_WRITE_ONLY) {
		// TODO(slavi): this is not just enough, there must be a way of syncing
		// so we know when the data was actually transferred
		// for now, just disable it so it's not accidently used
		// command list is not even running on multiple threads
		// so it's worthy implementing all of these at once
		//void* StagingBuffer = VBPool.Allocate(VertexBuffer, Offset, Size);
		//if (StagingBuffer != nullptr) {
		//	return StagingBuffer;
		//}
	}

	return VertexBuffer->Lock(Size, MemberOffset, LockMode, VertexBuffer->IsDynamicBuffer());
	RHI_THREAD_END_COMMAND_RETURN(void*);
}

void OpenGLDynamicRHI::RHIUnlockVertexBuffer(RHIVertexBuffer* VertexBufferRHI)
{
	RHI_THREAD_BEGIN_COMMAND();
	OpenGLVertexBuffer* VertexBuffer = StaticResourceCast(VertexBufferRHI);
	if (!VBPool.Deallocate(VertexBuffer)) {
		VertexBuffer->Unlock();
	}
	RHI_THREAD_END_COMMAND();
}

void OpenGLDynamicRHI::RHICopyVertexBuffer(RHIVertexBuffer* SourceBufferRHI, RHIVertexBuffer* DestBufferRHI)
{
	RHI_THREAD_BEGIN_COMMAND();
	OpenGLVertexBuffer* SourceBuffer = StaticResourceCast(SourceBufferRHI);
	OpenGLVertexBuffer* DestBuffer = StaticResourceCast(DestBufferRHI);

	OpenGLSpecs::BindBuffer(GL_COPY_READ_BUFFER, SourceBuffer->GetGLResource());
	OpenGLSpecs::BindBuffer(GL_COPY_WRITE_BUFFER, DestBuffer->GetGLResource());
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, SourceBuffer->GetSize());
	OpenGLSpecs::BindBuffer(GL_COPY_READ_BUFFER, 0);
	OpenGLSpecs::BindBuffer(GL_COPY_WRITE_BUFFER, 0);
	RHI_THREAD_END_COMMAND();
}