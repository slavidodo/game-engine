
#ifndef ENGINE_RHI_VERTEXFORMAT_H
#define ENGINE_RHI_VERTEXFORMAT_H

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
		: mSemantic(semantic), mOffset(offset), mType(type) {}

	OBJECT_GETACCESSOR(VertexElementSemantic, VertexElementSemantic, Semantic);
	OBJECT_GETACCESSOR(uint32_t, uint32_t, Offset);
	OBJECT_GETACCESSOR(VertexElementType, VertexElementType, Type);
	OBJECT_GETACCESSOR_DYNAMIC(size_t, size_t, Size, getTypeSize, mType);

	static uint32_t getTypeSize(VertexElementType type);
private:
	VertexElementSemantic mSemantic;
	uint32_t mOffset;
	VertexElementType mType;
};

class VertexFormat
{
public:
	VertexElement& addItem(VertexElementSemantic semantic, uint32_t offset, VertexElementType type) {
		mElements.emplace_back(semantic, offset, type);
		return mElements.back();
	}

private:
	std::vector<VertexElement> mElements;
};

#endif // ENGINE_RHI_VERTEXFORMAT_H
