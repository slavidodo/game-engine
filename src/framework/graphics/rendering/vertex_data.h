
#ifndef FRAMEWORK_GRAPHICS_CORE_RENDERING_VERTEX_DATA_H
#define FRAMEWORK_GRAPHICS_CORE_RENDERING_VERTEX_DATA_H

#include "vertex_format.h"

namespace framework {
namespace graphics {
namespace rendering {

class VertexData
{
public:
	VertexData()
		: m_vertexFormat(new VertexFormat) {}

	~VertexData() {
		delete m_vertexFormat;
	}

	OBJECT_ACCESSOR(size_t, size_t, vertexCount);
	OBJECT_GETACCESSOR(VertexFormat*, VertexFormat*, vertexFormat);

private:
	VertexFormat* m_vertexFormat = nullptr;

	size_t m_vertexCount;
};

} // ns rendering
} // ns graphics
} // ns framework

#endif // FRAMEWORK_GRAPHICS_CORE_RENDERING_VERTEX_FORMAT_H
