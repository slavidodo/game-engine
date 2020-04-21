
#ifndef FRAMEWORK_GRAPHICS_CORE_RENDERING_VERTEX_FORMAT_H
#define FRAMEWORK_GRAPHICS_CORE_RENDERING_VERTEX_FORMAT_H

namespace framework {
namespace graphics {
namespace rendering {

enum VertexElementSemantic
{
	VERTEX_ELEMENT_SEMANTIC_POSITION = 1,
	VERTEX_ELEMENT_SEMANTIC_DIFFUSE = 2,
	VERTEX_ELEMENT_SEMANTIC_SPECULAR = 3,
	VERTEX_ELEMENT_SEMANTIC_NORMAL = 4,
	VERTEX_ELEMENT_SEMANTIC_TEXCOORD = 5,
};

enum VertexElementType
{
	VERTEX_ELEMENT_TYPE_FLOAT1,
	VERTEX_ELEMENT_TYPE_FLOAT2,
	VERTEX_ELEMENT_TYPE_FLOAT3,
	VERTEX_ELEMENT_TYPE_FLOAT4,
};

class VertexElement
{
public:
	VertexElement(VertexElementSemantic semantic, uint32_t offset, VertexElementType type)
		: m_semantic(semantic), m_offset(offset), m_type(type) {}

	OBJECT_GETACCESSOR(VertexElementSemantic, VertexElementSemantic, semantic);
	OBJECT_GETACCESSOR(uint32_t, uint32_t, offset);
	OBJECT_GETACCESSOR(VertexElementType, VertexElementType, type);
	OBJECT_GETACCESSOR_DYNAMIC(size_t, size_t, size, getTypeSize, m_type);

	static uint32_t getTypeSize(VertexElementType type);
private:
	VertexElementSemantic m_semantic;
	uint32_t m_offset;
	VertexElementType m_type;
};

class VertexFormat
{
public:
	VertexElement& addItem(VertexElementSemantic semantic, uint32_t offset, VertexElementType type) {
		m_elements.emplace_back(semantic, offset, type);
		return m_elements.back();
	}

private:
	std::vector<VertexElement> m_elements;
};

} // ns rendering
} // ns graphics
} // ns framework

#endif // FRAMEWORK_GRAPHICS_CORE_RENDERING_VERTEX_FORMAT_H
