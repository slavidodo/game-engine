
#include "pch.h"

#include "OpenGL3RHIContext.h"
#include "OpenGL3RHIResource.h"

RHIIndexBuffer_ptr OpenGL3RHIContext::CreateIndexBuffer(size_t indicesCount, RHIIndexBufferType type, RHIHardwareBufferUsage usage)
{
	auto indexBuffer = std::shared_ptr<RHIIndexBuffer>(new OpenGL3RHIIndexBuffer(indicesCount, usage, type));
	return indexBuffer;
}

void* OpenGL3RHIContext::LockIndexBuffer(RHIIndexBuffer_ptr indexBuffer, size_t offset, size_t size, RHIResourceLockMode lockMode)
{
	return std::dynamic_pointer_cast<OpenGL3RHIIndexBuffer>(indexBuffer)
		->Lock(offset, size, lockMode, indexBuffer->CanBeDiscarded());
}

void OpenGL3RHIContext::UnlockIndexBuffer(RHIIndexBuffer_ptr indexBuffer)
{
	return std::dynamic_pointer_cast<OpenGL3RHIIndexBuffer>(indexBuffer)
		->Unlock();
}