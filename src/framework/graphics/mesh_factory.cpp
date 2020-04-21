
#include <framework/pch.h>

#include "mesh_factory.h"

#include <framework/graphics/rendering/vertex_data.h>
#include <framework/managers/hardware_buffer_manager.h>

using namespace framework::graphics;

framework::core::Mesh_ptr MeshFactory::createCube(float edgeLength /* = 100 */)
{
	const size_t totalVertices = 24; // 4 per face, 6 faces
	const size_t vertexEntrySize = 8; // pos(3) + normal(3) + uv(2)
	const size_t totalIndices = 36; // 6 per face, 6 faces (triangles)

	float vertices[totalVertices * vertexEntrySize] = {
		// front side
		-edgeLength, -edgeLength,  edgeLength,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		 edgeLength, -edgeLength,  edgeLength,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		 edgeLength,  edgeLength,  edgeLength,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		-edgeLength,  edgeLength,  edgeLength,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

		// back side
		 edgeLength, -edgeLength, -edgeLength,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-edgeLength, -edgeLength, -edgeLength,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-edgeLength,  edgeLength, -edgeLength,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		 edgeLength,  edgeLength, -edgeLength,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

		// left side
		-edgeLength, -edgeLength, -edgeLength, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-edgeLength, -edgeLength,  edgeLength, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-edgeLength,  edgeLength,  edgeLength, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-edgeLength,  edgeLength, -edgeLength, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		// right side
		 edgeLength, -edgeLength,  edgeLength,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 edgeLength, -edgeLength, -edgeLength,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 edgeLength,  edgeLength, -edgeLength,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 edgeLength,  edgeLength,  edgeLength,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		// up side
		-edgeLength,  edgeLength,  edgeLength,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		 edgeLength,  edgeLength,  edgeLength,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 edgeLength,  edgeLength, -edgeLength,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-edgeLength,  edgeLength, -edgeLength,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,

		// down side
		-edgeLength, -edgeLength, -edgeLength,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		 edgeLength, -edgeLength, -edgeLength,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		 edgeLength, -edgeLength,  edgeLength,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-edgeLength, -edgeLength,  edgeLength,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f };

	core::Mesh_ptr mesh = std::make_shared<core::Mesh>();
	mesh->sharedVertexData(std::make_shared<rendering::VertexData>());
	mesh->sharedVertexData()->vertexCount(totalVertices);

	auto vertexFormat = mesh->sharedVertexData()->vertexFormat();

	size_t offset = 0;
	offset += vertexFormat->addItem(rendering::VERTEX_ELEMENT_SEMANTIC_POSITION, offset, rendering::VERTEX_ELEMENT_TYPE_FLOAT3).size();
	offset += vertexFormat->addItem(rendering::VERTEX_ELEMENT_SEMANTIC_POSITION, offset, rendering::VERTEX_ELEMENT_TYPE_FLOAT3).size();
	offset += vertexFormat->addItem(rendering::VERTEX_ELEMENT_SEMANTIC_POSITION, offset, rendering::VERTEX_ELEMENT_TYPE_FLOAT2).size();
	
	size_t vertexSize = offset;
	auto vertexBuffer = managers::HardwareBufferManager::instance().createVertexBuffer(totalVertices, vertexSize,
																					   rendering::HardwareBuffer::USAGE_STATIC_WRITE_ONLY);
	vertexBuffer->write((void*)vertices, vertexBuffer->size(), 0, true);

	// TODO; hell no, there are a lot more to do
	// this buffer wont really draw itself
	// index buffer

	return mesh;
}