
#include "pch.h"

#include "OpenGl3RHIBufferImpl.h"

OpenGl3RHIBufferImpl::OpenGl3RHIBufferImpl(GLenum target, RHIHardwareBufferUsage usage, size_t size)
	: RHIHardwareBuffer(usage), mTarget(target)
{
	mSize = size;
	if (usage & RHIHardwareBufferUsage::HWBU_DISCARD) {
		mGlUsage = GL_STREAM_DRAW;
	} else if (usage & RHIHardwareBufferUsage::HWBU_STATIC) {
		mGlUsage = GL_STATIC_DRAW;
	} else {
		mGlUsage = GL_DYNAMIC_DRAW;
	}

	glGenBuffers(1, &mBufferId);
}

void OpenGl3RHIBufferImpl::ReadData(size_t size, size_t offset, void* dest)
{
	Bind();
	glGetBufferSubData(mTarget, offset, size, dest);
}

void OpenGl3RHIBufferImpl::WriteData(void* src, size_t size, size_t offset, bool discard)
{
	Bind();
	if (offset == 0 || size == mSize) {
		glBufferData(mTarget, size, src, mGlUsage);
	} else if (discard) {
		glBufferData(mTarget, size, nullptr, mGlUsage);
	} else {
		glBufferSubData(mTarget, offset, size, src);
	}
}

void OpenGl3RHIBufferImpl::Bind()
{
	// TODO; use global state manager?
	glBindBuffer(mTarget, mBufferId);
}