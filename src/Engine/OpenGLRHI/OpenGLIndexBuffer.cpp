
#include "pch.h"

#include "OpenGLDynamicRHI.h"
#include "OpenGLResource.h"

RHIIndexBufferRef OpenGLDynamicRHI::RHICreateIndexBuffer(size_t IndicesCount, RHIIndexBufferType Type, RHIHardwareBufferUsage Usage)
{
	RHIIndexBufferRef IndexBuffer(new OpenGLIndexBuffer(IndicesCount, Usage, Type));
	return IndexBuffer;
}

void* OpenGLDynamicRHI::RHILockIndexBuffer(RHIIndexBuffer* IndexBuffer, size_t MemberOffset, size_t Size, RHIResourceLockMode LockMode)
{
	RHI_THREAD_BEGIN_COMMAND();
	return StaticResourceCast(IndexBuffer)->Lock(Size, MemberOffset, LockMode, IndexBuffer->IsDynamicBuffer());
	RHI_THREAD_END_COMMAND_RETURN(void*);
}

void OpenGLDynamicRHI::RHIUnlockIndexBuffer(RHIIndexBuffer* indexBuffer)
{
	RHI_THREAD_BEGIN_COMMAND();
	return StaticResourceCast(indexBuffer)->Unlock();
	RHI_THREAD_END_COMMAND();
}