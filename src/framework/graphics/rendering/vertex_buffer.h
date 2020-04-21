
#ifndef FRAMEWORK_GRAPHICS_RENDERING_VERTEX_BUFFER_H
#define FRAMEWORK_GRAPHICS_RENDERING_VERTEX_BUFFER_H

#include "hardware_buffer.h"
#include <framework/graphics/rendering/vertex_format.h>

namespace framework {
namespace graphics {
namespace rendering {

// TODO; clean up and support instancing
class VertexBuffer : public HardwareBuffer
{
public:
	VertexBuffer(size_t verticesCount, size_t vertexSize, Usage usage)
		: HardwareBuffer(usage) {
		m_size = verticesCount * vertexSize;
	}

	OBJECT_GETACCESSOR(size_t, size_t, verticesCount);
	OBJECT_GETACCESSOR(size_t, size_t, vertexSize);

private:
	size_t m_verticesCount;
	size_t m_vertexSize;
};

} // ns rendering
} // ns graphics
} // ns framework

#endif // FRAMEWORK_GRAPHICS_RENDERING_VERTEX_BUFFER_H
