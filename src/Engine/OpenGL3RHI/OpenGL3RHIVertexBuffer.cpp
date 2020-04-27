
#include "pch.h"

#include "OpenGL3RHIContext.h"
#include "OpenGL3RHIResource.h"

RHIVertexBuffer_ptr OpenGL3RHIContext::CreateVertexBuffer(size_t verticesCount, size_t vertexSize, RHIHardwareBufferUsage usage)
{
	auto vertexBuffer = std::shared_ptr<RHIVertexBuffer>(new OpenGL3RHIVertexBuffer(verticesCount, vertexSize, usage));
	// TODO (Data, Buffer, ...)
	return vertexBuffer;
}

void* OpenGL3RHIContext::LockVertexBuffer(RHIVertexBuffer_ptr vertexBuffer, size_t offset, size_t size)
{
	return nullptr;
}

void OpenGL3RHIContext::UnlockVertexBuffer(RHIVertexBuffer_ptr vertexBuffer)
{

}