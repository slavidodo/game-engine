
#include "pch.h"

#include "opengl_hardware_buffer_impl.h"

using namespace framework::graphics::opengl;

OpenGLHardwareBufferImpl::OpenGLHardwareBufferImpl(GLenum target, Usage usage, size_t size)
	: HardwareBuffer(usage), m_target(target)
{
	m_size = size;
	if (usage & HardwareBuffer::USAGE_DISCARD) {
		m_glUsage = GL_STREAM_DRAW;
	} else if (usage & HardwareBuffer::USAGE_STATIC) {
		m_glUsage = GL_STATIC_DRAW;
	} else {
		m_glUsage = GL_DYNAMIC_DRAW;
	}

	glGenBuffers(1, &m_bufferId);
}

void OpenGLHardwareBufferImpl::read(size_t size, size_t offset, void* dest)
{
	bind();
	glGetBufferSubData(m_target, offset, size, dest);
}

void OpenGLHardwareBufferImpl::write(void* src, size_t size, size_t offset, bool discard)
{
	bind();
	if (offset == 0 || size == m_size) {
		glBufferData(m_target, size, src, m_glUsage);
	} else if (discard) {
		glBufferData(m_target, size, nullptr, m_glUsage);
	} else {
		glBufferSubData(m_target, offset, size, src);
	}
}

void OpenGLHardwareBufferImpl::bind()
{
	// TODO; use global state manager?
	glBindBuffer(m_target, m_bufferId);
}