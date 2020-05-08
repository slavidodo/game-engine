
#include "pch.h"

#include "OpenGLDynamicRHI.h"
#include "OpenGLResource.h"

RHIIndexBuffer_ptr OpenGLDynamicRHI::RHICreateIndexBuffer(size_t IndicesCount, RHIIndexBufferType Type, RHIHardwareBufferUsage Usage)
{
	RHIIndexBuffer_ptr IndexBuffer(new OpenGLIndexBuffer(IndicesCount, Usage, Type));
	return IndexBuffer;
}

void* OpenGLDynamicRHI::RHILockIndexBuffer(RHIIndexBuffer_ptr IndexBuffer, size_t Offset, size_t Size, RHIResourceLockMode LockMode)
{
	return std::dynamic_pointer_cast<OpenGLIndexBuffer>(IndexBuffer)
		->Lock(Size, Offset, LockMode, IndexBuffer->CanBeDiscarded());
}

void OpenGLDynamicRHI::RHIUnlockIndexBuffer(RHIIndexBuffer_ptr indexBuffer)
{
	return std::dynamic_pointer_cast<OpenGLIndexBuffer>(indexBuffer)
		->Unlock();
}