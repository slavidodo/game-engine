
#include <framework/pch.h>

#include "opengl_vertex_buffer.h"

using namespace framework::graphics::opengl;


void OpenGLVertexBuffer::read(size_t size, size_t offset, void* dest)
{
	m_bufferImpl->read(size, offset, dest);
}

void OpenGLVertexBuffer::write(void* src, size_t size, size_t offset, bool discard)
{
	m_bufferImpl->write(src, size, offset, discard);
}