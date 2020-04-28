
#include "pch.h"

#include "OpenGL3RHIContext.h"
#include "OpenGL3RHIResource.h"

namespace
{

// We could implement a pool allocator here
void* AllocateStagingBuffer(OpenGL3RHIVertexBuffer_ptr vertexBuffer, size_t size, size_t offset)
{
	// TODO use modern techniques if available
	// this avoids a stall when transferring data
	// this is commonly called as staging buffer
	// (GL_ARB_buffer_storage || >= opengl 4.4)

	return nullptr;
}

bool DeallocateStagingBuffer(OpenGL3RHIVertexBuffer_ptr vertexBuffer)
{
	// TODO
	return false;
}

}

RHIVertexBuffer_ptr OpenGL3RHIContext::CreateVertexBuffer(size_t verticesCount, size_t vertexSize, RHIHardwareBufferUsage usage)
{
	auto vertexBuffer = std::shared_ptr<RHIVertexBuffer>(new OpenGL3RHIVertexBuffer(verticesCount, vertexSize, usage));
	// TODO (Data, Buffer, ...)
	return vertexBuffer;
}

void* OpenGL3RHIContext::LockVertexBuffer(RHIVertexBuffer_ptr vertexBuffer, size_t offset, size_t size, RHIResourceLockMode lockMode)
{
	auto VertexBuffer = std::dynamic_pointer_cast<OpenGL3RHIVertexBuffer>(vertexBuffer);

	// TODO: VAB (gl 4.3) when no stride is present
	// TODO: use a storage buffer
	if (vertexBuffer->CanBeDiscarded() && lockMode == RHIResourceLockMode::RLM_WRITE_ONLY) {
		void* StagingBuffer = AllocateStagingBuffer(VertexBuffer, size, offset);
		if (StagingBuffer != NULL) {
			return StagingBuffer;
		}
	}

	return VertexBuffer->Lock(size, offset, lockMode, vertexBuffer->CanBeDiscarded());
}

void OpenGL3RHIContext::UnlockVertexBuffer(RHIVertexBuffer_ptr vertexBuffer)
{
	auto VertexBuffer = std::dynamic_pointer_cast<OpenGL3RHIVertexBuffer>(vertexBuffer);
	if (!DeallocateStagingBuffer(VertexBuffer)) {
		VertexBuffer->Unlock();
	}
}

void OpenGL3RHIContext::CopyVertexBuffer(RHIVertexBuffer_ptr sourceBuffer, RHIVertexBuffer_ptr destBuffer)
{
	OpenGL3RHIVertexBuffer_ptr SourceBuffer = std::dynamic_pointer_cast<OpenGL3RHIVertexBuffer>(sourceBuffer);
	OpenGL3RHIVertexBuffer_ptr DestBuffer = std::dynamic_pointer_cast<OpenGL3RHIVertexBuffer>(destBuffer);

	glBindBuffer(GL_COPY_READ_BUFFER, SourceBuffer->GetBufferId());
	glBindBuffer(GL_COPY_WRITE_BUFFER, DestBuffer->GetBufferId());
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, SourceBuffer->GetSize());
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}