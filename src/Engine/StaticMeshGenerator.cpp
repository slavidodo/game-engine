
#include "pch.h"

#include "StaticMeshGenerator.h"

Mesh_ptr StaticMeshGenerator::CreateCube(float edgeLength /* = 100 */)
{
	auto mesh = std::make_shared<Mesh>();
	CreateCube(mesh, edgeLength);
	return mesh;
}

void StaticMeshGenerator::CreateCube(Mesh_ptr mesh, float edgeLength /* = 100 */)
{
	const size_t totalVertices = 24; // 4 per face * 6 faces
	const size_t vertexEntrySize = 8; // pos(3) + normal(3) + uv(2)
	const float vertices[totalVertices * vertexEntrySize] = {
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

	const size_t totalIndices = 36; // 3 per trig * 2 per face * 6 faces (triangles)
	const float indices[totalIndices] = {
		// front
		0, 1, 2, 0, 2, 3,
		
		// back
		4, 5, 6, 4, 6, 7,

		// left
		8, 9, 10, 8, 10, 11,

		// right
		12, 13, 14, 12, 14, 15,

		// up
		16, 17, 18, 16, 18, 19,

		// back
		20, 21, 22, 20, 22, 23 };


}