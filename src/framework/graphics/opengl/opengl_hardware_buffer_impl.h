
#ifndef FRAMEWORK_GRAPHICS_OPENGL_HARDWARE_BUFFER_IMPL_H
#define FRAMEWORK_GRAPHICS_OPENGL_HARDWARE_BUFFER_IMPL_H

#include <framework/graphics/rendering/hardware_buffer.h>
#include <GL/glew.h>

namespace framework {
namespace graphics {
namespace opengl {

class OpenGLHardwareBufferImpl : public rendering::HardwareBuffer
{
public:
	OpenGLHardwareBufferImpl(GLenum target, Usage usage, size_t size);
	OpenGLHardwareBufferImpl(OpenGLHardwareBufferImpl&) = delete;

	void read(size_t size, size_t offset, void* dest) override final;
	void write(void* src, size_t size, size_t offset, bool discard) override final;

private:
	void bind();

	GLenum m_glUsage;
	GLenum m_target;
	GLuint m_bufferId;
};

} // ns rendering
} // ns graphics
} // ns framework

#endif // FRAMEWORK_GRAPHICS_OPENGL_HARDWARE_BUFFER_H