
#ifndef FRAMEWORK_GRAPHICS_OPENGL_HARDWARE_BUFFER_MANAGER_H
#define FRAMEWORK_GRAPHICS_OPENGL_HARDWARE_BUFFER_MANAGER_H

#include "opengl_hardware_buffer_impl.h"
#include "opengl_index_buffer.h"
#include "opengl_vertex_buffer.h"

#include <framework/managers/hardware_buffer_manager.h>

namespace framework {
namespace graphics {
namespace opengl {

class OpenGLHardwareBufferManager : public managers::HardwareBufferManager
{
public:

	VertexBuffer* createVertexBuffer(size_t verticesCount, size_t vertexSize, HardwareBuffer::Usage usage) override final;
	IndexBuffer* createIndexBuffer(size_t indicesCount, rendering::HardwareBuffer::Usage usage) override final;

private:
};

} // ns rendering
} // ns graphics
} // ns framework

#endif // FRAMEWORK_GRAPHICS_OPENGL_INDEX_BUFFER_H
