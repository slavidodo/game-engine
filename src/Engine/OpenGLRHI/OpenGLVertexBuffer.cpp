
#include "pch.h"

#include "OpenGLDynamicRHI.h"
#include "OpenGLResource.h"

namespace
{

// We could implement a pool allocator here
void* AllocateStagingBuffer(OpenGLVertexBuffer_ptr vertexBuffer, size_t size, size_t offset)
{
	// TODO use modern techniques if available
	// this avoids a stall when transferring data
	// this is commonly called as staging buffer
	// (GL_ARB_buffer_storage || >= opengl 4.4)

	return nullptr;
}

bool DeallocateStagingBuffer(OpenGLVertexBuffer_ptr vertexBuffer)
{
	// TODO
	return false;
}

}

RHIVertexBuffer_ptr OpenGLDynamicRHI::RHICreateVertexBuffer(size_t VerticesCount, size_t VertexSize, RHIHardwareBufferUsage Usage)
{
	RHIVertexBuffer_ptr vertexBuffer(new OpenGLVertexBuffer(VerticesCount, VertexSize, Usage));
	// TODO (Data, Buffer, ...)
	return vertexBuffer;
}

void* OpenGLDynamicRHI::RHILockVertexBuffer(RHIVertexBuffer_ptr VertexBufferRHI, size_t Offset, size_t Size, RHIResourceLockMode LockMode)
{
	auto VertexBuffer = std::dynamic_pointer_cast<OpenGLVertexBuffer>(VertexBufferRHI);

	// TODO: VAB (gl 4.3) when no stride is present
	// TODO: use a storage buffer
	if (VertexBuffer->CanBeDiscarded() && LockMode == RHIResourceLockMode::RLM_WRITE_ONLY) {
		void* StagingBuffer = AllocateStagingBuffer(VertexBuffer, Size, Offset);
		if (StagingBuffer != nullptr) {
			return StagingBuffer;
		}
	}

	return VertexBuffer->Lock(Size, Offset, LockMode, VertexBuffer->CanBeDiscarded());
}

void OpenGLDynamicRHI::RHIUnlockVertexBuffer(RHIVertexBuffer_ptr VertexBufferRHI)
{
	auto VertexBuffer = std::dynamic_pointer_cast<OpenGLVertexBuffer>(VertexBufferRHI);
	if (!DeallocateStagingBuffer(VertexBuffer)) {
		VertexBuffer->Unlock();
	}
}

void OpenGLDynamicRHI::RHICopyVertexBuffer(RHIVertexBuffer_ptr SourceBufferRHI, RHIVertexBuffer_ptr DestBufferRHI)
{
	auto SourceBuffer = std::dynamic_pointer_cast<OpenGLVertexBuffer>(SourceBufferRHI);
	auto DestBuffer = std::dynamic_pointer_cast<OpenGLVertexBuffer>(DestBufferRHI);

	glBindBuffer(GL_COPY_READ_BUFFER, SourceBuffer->GetBufferId());
	glBindBuffer(GL_COPY_WRITE_BUFFER, DestBuffer->GetBufferId());
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, SourceBuffer->GetSize());
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}