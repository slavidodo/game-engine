
#include "pch.h"

#include "OpenGl3RHIBufferImpl.h"

OpenGl3RHIBufferImpl::OpenGl3RHIBufferImpl(GLenum target, RHIHardwareBufferUsage usage, size_t size)
	: RHIHardwareBuffer(usage), m_target(target)
{
	m_size = size;
	if (usage & RHIHardwareBufferUsage::HWBU_DISCARD) {
		m_glUsage = GL_STREAM_DRAW;
	} else if (usage & RHIHardwareBufferUsage::HWBU_STATIC) {
		m_glUsage = GL_STATIC_DRAW;
	} else {
		m_glUsage = GL_DYNAMIC_DRAW;
	}

	glGenBuffers(1, &m_bufferId);
}

void OpenGl3RHIBufferImpl::ReadData(size_t size, size_t offset, void* dest)
{
	Bind();
	glGetBufferSubData(m_target, offset, size, dest);
}

void OpenGl3RHIBufferImpl::WriteData(void* src, size_t size, size_t offset, bool discard)
{
	Bind();
	if (offset == 0 || size == m_size) {
		glBufferData(m_target, size, src, m_glUsage);
	} else if (discard) {
		glBufferData(m_target, size, nullptr, m_glUsage);
	} else {
		glBufferSubData(m_target, offset, size, src);
	}
}

void OpenGl3RHIBufferImpl::Bind()
{
	// TODO; use global state manager?
	glBindBuffer(m_target, m_bufferId);
}