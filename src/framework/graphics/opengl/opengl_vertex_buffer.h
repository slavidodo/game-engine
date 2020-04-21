
#ifndef FRAMEWORK_GRAPHICS_OPENGL_VERTEX_BUFFER_H
#define FRAMEWORK_GRAPHICS_OPENGL_VERTEX_BUFFER_H

#include <framework/graphics/rendering/vertex_buffer.h>
#include "opengl_hardware_buffer_impl.h"

namespace framework {
namespace graphics {
namespace opengl {

class OpenGLVertexBuffer : public rendering::VertexBuffer
{
public:
	OpenGLVertexBuffer(size_t verticesCount, size_t vertexSize, Usage usage)
		: VertexBuffer(verticesCount, vertexSize, usage) {
		m_bufferImpl.reset(new OpenGLHardwareBufferImpl(GL_ARRAY_BUFFER, usage, m_size));
	}

	void read(size_t size, size_t offset, void* dest) override final;
	void write(void* src, size_t size, size_t offset, bool discard) override final;

private:
	std::unique_ptr<OpenGLHardwareBufferImpl> m_bufferImpl;
};

} // ns opengl
} // ns graphics
} // ns framework

#endif
