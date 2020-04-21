
#ifndef FRAMEWORK_GRAPHICS_OPENGL_INDEX_BUFFER_H
#define FRAMEWORK_GRAPHICS_OPENGL_INDEX_BUFFER_H

#include <framework/graphics/rendering/index_buffer.h>
#include "opengl_hardware_buffer_impl.h"

namespace framework {
namespace graphics {
namespace opengl {

class OpenGLIndexBuffer : public rendering::IndexBuffer
{
public:
	OpenGLIndexBuffer(size_t indicesCount, Usage usage)
		: IndexBuffer(indicesCount, usage) {
		m_bufferImpl.reset(new OpenGLHardwareBufferImpl(GL_ELEMENT_ARRAY_BUFFER, usage, m_size));
	}

	constexpr size_t getSize() {
		return sizeof(size_t);
	}

	void read(size_t size, size_t offset, void* dest) override final;
	void write(void* data, size_t size, size_t offset, bool discard) override final;
private:
	std::unique_ptr<OpenGLHardwareBufferImpl> m_bufferImpl;
};

} // ns rendering
} // ns graphics
} // ns framework

#endif // FRAMEWORK_GRAPHICS_OPENGL_INDEX_BUFFER_H
