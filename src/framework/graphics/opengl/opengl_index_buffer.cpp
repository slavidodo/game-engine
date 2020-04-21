
#include <framework/pch.h>

#include "opengl_index_buffer.h"

using namespace framework::graphics::opengl;

void OpenGLIndexBuffer::read(size_t size, size_t offset, void* dest)
{
	m_bufferImpl->read(size, offset, dest);
}

void OpenGLIndexBuffer::write(void* src, size_t size, size_t offset, bool discard)
{
	m_bufferImpl->write(src, size, offset, discard);
}