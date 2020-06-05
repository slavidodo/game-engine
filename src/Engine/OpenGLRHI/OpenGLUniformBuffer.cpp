
#include "pch.h"

#include "OpenGLDynamicRHI.h"

extern OpenGLDynamicRHI* PrivateOpenGLDevice;

namespace
{

#define NUM_POOL_BUCKETS 45

static const uint32_t RequestedUniformBufferSizeBuckets[NUM_POOL_BUCKETS] = {
	16,32,48,64,80,96,112,128,	// 16-byte increments
	160,192,224,256,			// 32-byte increments
	320,384,448,512,			// 64-byte increments
	640,768,896,1024,			// 128-byte increments
	1280,1536,1792,2048,		// 256-byte increments
	2560,3072,3584,4096,		// 512-byte increments
	5120,6144,7168,8192,		// 1024-byte increments
	10240,12288,14336,16384,	// 2048-byte increments
	20480,24576,28672,32768,	// 4096-byte increments
	40960,49152,57344,65536,	// 8192-byte increments

	// 65536 is current max uniform buffer size for Mac OS X.

	0xFFFF0000 // Not max uint32 to allow rounding
};

static std::vector<uint32_t> UniformBufferSizeBuckets;

uint32_t GetPoolBucketIndex(uint32_t BufferSize)
{
	if (UniformBufferSizeBuckets.size() == 0) {
		GLint Alignment;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &Alignment);

		for (int32_t Count = 0; Count < NUM_POOL_BUCKETS; Count++) {
			//uint32_t AlignedSize = Align(RequestedUniformBufferSizeBuckets[Count], Alignment);
			uint32_t AlignedSize = ((RequestedUniformBufferSizeBuckets[Count] + Alignment - 1) / Alignment) * Alignment;
			if (std::find(UniformBufferSizeBuckets.begin(), UniformBufferSizeBuckets.end(), AlignedSize) == UniformBufferSizeBuckets.end()) {
				UniformBufferSizeBuckets.push_back(AlignedSize);
			}
		}
	}

	assert(UniformBufferSizeBuckets.size() > 0);

	auto it = std::lower_bound(UniformBufferSizeBuckets.begin(), UniformBufferSizeBuckets.end(), BufferSize);
	return it - UniformBufferSizeBuckets.begin();
}

void CopyDataToUniformBuffer(OpenGLUniformBuffer* UniformBuffer, const void* Contents, uint32_t ContentSize)
{
	// todo: emulated buffers
	glBufferSubData(GL_UNIFORM_BUFFER, 0, ContentSize, Contents);
}

static void SetLayoutTable(OpenGLUniformBuffer* UniformBuffer, const void* Contents, const RHIUniformBufferLayout& Layout, EUniformBufferValidation Validation)
{
	if (Layout.Resources.size()) {
		size_t NumResources = Layout.Resources.size();
		UniformBuffer->ResourceTable.resize(NumResources, nullptr);
		for (size_t i = 0; i < NumResources; ++i) {
			//RHIResource* Resource = *(RHIResource**)((uint8_t*)Contents + Layout.Resources[i].MemberOffset);
			//UniformBuffer->ResourceTable[i] = Resource;
		}
	}
}

void UpdateUniformBufferContents(OpenGLUniformBuffer* UniformBuffer, const void* Contents, uint32_t ConstantBufferSize)
{
	if (ConstantBufferSize > 0) {
		PrivateOpenGLDevice->CachedBindUniformBuffer(PrivateOpenGLDevice->GetContextStateForCurrentContext(),
													 UniformBuffer->Resource);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, ConstantBufferSize, Contents);
	}
}

}

RHIUniformBufferRef CreateUniformBuffer(const void* Contents, const RHIUniformBufferLayout& Layout, EUniformBufferUsage Usage, EUniformBufferValidation Validation)
{
	// TODO(slavi) multiple threading support

	OpenGLUniformBuffer* UniformBuffer = new OpenGLUniformBuffer(Layout);

	const uint32_t BucketIndex = GetPoolBucketIndex(Layout.ConstantBufferSize);
	const uint32_t SizeOfBufferToAllocate = UniformBufferSizeBuckets[BucketIndex];
	const uint32_t AllocatedSize = (SizeOfBufferToAllocate > 0) ? SizeOfBufferToAllocate : Layout.ConstantBufferSize;;

	const bool StreamDraw = (Usage == UniformBuffer_SingleDraw || Usage == UniformBuffer_SingleFrame);

	std::function<void(void)> GLCreationFunc;

	if (!OpenGLSpecs::SupportsUniformBuffers()) {
		// TODO: emulate ubos to work like typical globals
	} else {
		GLCreationFunc = [&]() {
			GLuint AllocatedResource;
			uint32_t OffsetInBuffer = 0;
			glGenBuffers(1, &AllocatedResource);

			PrivateOpenGLDevice->CachedBindUniformBuffer(PrivateOpenGLDevice->GetContextStateForCurrentContext(),
														 AllocatedResource);
			glBufferData(GL_UNIFORM_BUFFER, AllocatedSize, NULL, StreamDraw ? GL_STREAM_DRAW : GL_STATIC_DRAW);
			UniformBuffer->SetGLUniformBufferParams(AllocatedResource, OffsetInBuffer, AllocatedSize, StreamDraw);
		};
	}

	GLCreationFunc();

	CopyDataToUniformBuffer(UniformBuffer, Contents, Layout.ConstantBufferSize);

	// Initialize the resource table for this uniform buffer.
	SetLayoutTable(UniformBuffer, Contents, Layout, Validation);

	return UniformBuffer;
}

RHIUniformBufferRef OpenGLDynamicRHI::RHICreateUniformBuffer(const void* Contents, const RHIUniformBufferLayout& Layout, EUniformBufferUsage Usage, EUniformBufferValidation Validation)
{
	// if the buffer contains constants
	if (Layout.ConstantBufferSize > 0) {
		// TODO use pooling
	}

	return CreateUniformBuffer(Contents, Layout, Usage, Validation);
}

void OpenGLDynamicRHI::RHIUpdateUniformBuffer(RHIUniformBuffer* UniformBufferRHI, const void* Contents)
{
	OpenGLUniformBuffer* UniformBuffer = StaticResourceCast(UniformBufferRHI);
	const RHIUniformBufferLayout& Layout = UniformBufferRHI->GetLayout();

	const size_t ConstantBufferSize = Layout.ConstantBufferSize;
	const size_t NumResources = Layout.Resources.size();

	UpdateUniformBufferContents(UniformBuffer, Contents, ConstantBufferSize);

	for (size_t ResourceIndex = 0; ResourceIndex < NumResources; ++ResourceIndex) {
		//RHIResource* Resource = *(RHIResource**)((uint8_t*)Contents + Layout.Resources[ResourceIndex].MemberOffset);

		// TODO(slavi) validate resource
		//UniformBuffer->ResourceTable[ResourceIndex] = Resource;
	}
}