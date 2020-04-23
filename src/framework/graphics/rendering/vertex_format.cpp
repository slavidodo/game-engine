
#include "pch.h"

#include "vertex_format.h"

using namespace framework::graphics::rendering;

uint32_t VertexElement::getTypeSize(VertexElementType type)
{
	switch (type) {
		case VERTEX_ELEMENT_TYPE_FLOAT1: return sizeof(float);
		case VERTEX_ELEMENT_TYPE_FLOAT2: return sizeof(float) * 2;
		case VERTEX_ELEMENT_TYPE_FLOAT3: return sizeof(float) * 3;
		case VERTEX_ELEMENT_TYPE_FLOAT4: return sizeof(float) * 4;
	}

	return 0;
}