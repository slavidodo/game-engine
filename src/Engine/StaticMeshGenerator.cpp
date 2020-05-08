
#include "pch.h"

#include "StaticMeshGenerator.h"

#include "RHI/RHICommandList.h"

StaticMesh_ptr StaticMeshGenerator::CreateTriangle(float edgeLength)
{
	auto mesh = std::make_shared<StaticMesh>();

	const size_t NumVertices = 4 * 6;
	const size_t NumIndices = 3;

	edgeLength /= 2.0f;

	StaticMeshVertex Vertices[NumVertices] = {
		{glm::fvec3(-edgeLength, -edgeLength, 0.0f),  glm::fvec3(1.0f,  0.0f,  0.0f), glm::fvec2(0.0f, 1.0f)},
		{glm::fvec3( edgeLength, -edgeLength, 0.0f),  glm::fvec3(0.0f,  1.0f,  0.0f), glm::fvec2(1.0f, 1.0f)},
		{glm::fvec3(       0.0f,  edgeLength, 0.0f),  glm::fvec3(0.0f,  0.0f,  1.0f), glm::fvec2(1.0f, 0.0f)},
	};

	uint16_t Indices[NumIndices] = {
		0, 1, 2
	};

	StaticMeshCreateInfo createInfo;
	createInfo.NumVertices = NumVertices;
	createInfo.NumIndices = NumIndices;
	createInfo.Vertices = Vertices;
	createInfo.Indices = Indices;
	createInfo.IndexType = RHIIndexBufferType::IBT_16;

	CreateStaticMeshWithDefaultFilter(mesh, createInfo);
	return mesh;
}

StaticMesh_ptr StaticMeshGenerator::CreateQuad(float edgeLength)
{
	auto mesh = std::make_shared<StaticMesh>();

	const size_t NumVertices = 4 * 6;
	const size_t NumIndices = 6;

	edgeLength /= 2.0f;

	StaticMeshVertex Vertices[NumVertices] = {
		{glm::fvec3( edgeLength,  edgeLength, 0.0f),  glm::fvec3(1.0f,  0.0f,  0.0f), glm::fvec2(0.0f, 1.0f)},
		{glm::fvec3( edgeLength, -edgeLength, 0.0f),  glm::fvec3(0.0f,  1.0f,  0.0f), glm::fvec2(1.0f, 1.0f)},
		{glm::fvec3(-edgeLength, -edgeLength, 0.0f),  glm::fvec3(0.0f,  0.0f,  1.0f), glm::fvec2(1.0f, 0.0f)},
		{glm::fvec3(-edgeLength,  edgeLength, 0.0f),  glm::fvec3(1.0f,  0.0f,  1.0f), glm::fvec2(0.0f, 0.0f)},
	};

	uint16_t Indices[NumIndices] = {
		0, 1, 3,
		1, 2, 3
	};

	StaticMeshCreateInfo createInfo;
	createInfo.NumVertices = NumVertices;
	createInfo.NumIndices = NumIndices;
	createInfo.Vertices = Vertices;
	createInfo.Indices = Indices;
	createInfo.IndexType = RHIIndexBufferType::IBT_16;

	CreateStaticMeshWithDefaultFilter(mesh, createInfo);
	return mesh;
}

StaticMesh_ptr StaticMeshGenerator::CreateCube(float edgeLength /* = 1.0f */)
{
	auto mesh = std::make_shared<StaticMesh>();

	// initial information
	const size_t NumVertices = 4 * 6;
	const size_t NumIndices = 2 * 6 * 3;

	StaticMeshVertex Vertices[NumVertices] = {
		// front side
		{glm::fvec3(-edgeLength, -edgeLength,  edgeLength),  glm::fvec3(0.0f,  0.0f,  1.0f), glm::fvec2(0.0f, 1.0f)},
		{glm::fvec3( edgeLength, -edgeLength,  edgeLength),  glm::fvec3(0.0f,  0.0f,  1.0f), glm::fvec2(1.0f, 1.0f)},
		{glm::fvec3( edgeLength,  edgeLength,  edgeLength),  glm::fvec3(0.0f,  0.0f,  1.0f), glm::fvec2(1.0f, 0.0f)},
		{glm::fvec3(-edgeLength,  edgeLength,  edgeLength),  glm::fvec3(0.0f,  0.0f,  1.0f), glm::fvec2(0.0f, 0.0f)},

		// back side
		{glm::fvec3( edgeLength, -edgeLength, -edgeLength),  glm::fvec3(0.0f,  0.0f, -1.0f), glm::fvec2(0.0f, 1.0f)},
		{glm::fvec3(-edgeLength, -edgeLength, -edgeLength),  glm::fvec3(0.0f,  0.0f, -1.0f), glm::fvec2(1.0f, 1.0f)},
		{glm::fvec3(-edgeLength,  edgeLength, -edgeLength),  glm::fvec3(0.0f,  0.0f, -1.0f), glm::fvec2(1.0f, 0.0f)},
		{glm::fvec3( edgeLength,  edgeLength, -edgeLength),  glm::fvec3(0.0f,  0.0f, -1.0f), glm::fvec2(0.0f, 0.0f)},

		// left side
		{glm::fvec3(-edgeLength, -edgeLength, -edgeLength), glm::fvec3(-1.0f,  0.0f,  0.0f), glm::fvec2(0.0f, 1.0f)},
		{glm::fvec3(-edgeLength, -edgeLength,  edgeLength), glm::fvec3(-1.0f,  0.0f,  0.0f), glm::fvec2(1.0f, 1.0f)},
		{glm::fvec3(-edgeLength,  edgeLength,  edgeLength), glm::fvec3(-1.0f,  0.0f,  0.0f), glm::fvec2(1.0f, 0.0f)},
		{glm::fvec3(-edgeLength,  edgeLength, -edgeLength), glm::fvec3(-1.0f,  0.0f,  0.0f), glm::fvec2(0.0f, 0.0f)},

		// right side
		{glm::fvec3( edgeLength, -edgeLength,  edgeLength), glm::fvec3( 1.0f,  0.0f,  0.0f), glm::fvec2(0.0f, 1.0f)},
		{glm::fvec3( edgeLength, -edgeLength, -edgeLength), glm::fvec3( 1.0f,  0.0f,  0.0f), glm::fvec2(1.0f, 1.0f)},
		{glm::fvec3( edgeLength,  edgeLength, -edgeLength), glm::fvec3( 1.0f,  0.0f,  0.0f), glm::fvec2(1.0f, 0.0f)},
		{glm::fvec3( edgeLength,  edgeLength,  edgeLength), glm::fvec3( 1.0f,  0.0f,  0.0f), glm::fvec2(0.0f, 0.0f)},

		// up side
		{glm::fvec3(-edgeLength,  edgeLength,  edgeLength), glm::fvec3( 0.0f,  1.0f,  0.0f), glm::fvec2(0.0f, 1.0f)},
		{glm::fvec3( edgeLength,  edgeLength,  edgeLength), glm::fvec3( 0.0f,  1.0f,  0.0f), glm::fvec2(1.0f, 1.0f)},
		{glm::fvec3( edgeLength,  edgeLength, -edgeLength), glm::fvec3( 0.0f,  1.0f,  0.0f), glm::fvec2(1.0f, 0.0f)},
		{glm::fvec3(-edgeLength,  edgeLength, -edgeLength), glm::fvec3( 0.0f,  1.0f,  0.0f), glm::fvec2(0.0f, 0.0f)},

		// down side
		{glm::fvec3(-edgeLength, -edgeLength, -edgeLength), glm::fvec3( 0.0f, -1.0f,  0.0f), glm::fvec2(0.0f, 1.0f)},
		{glm::fvec3( edgeLength, -edgeLength, -edgeLength), glm::fvec3( 0.0f, -1.0f,  0.0f), glm::fvec2(1.0f, 1.0f)},
		{glm::fvec3( edgeLength, -edgeLength,  edgeLength), glm::fvec3( 0.0f, -1.0f,  0.0f), glm::fvec2(1.0f, 0.0f)},
		{glm::fvec3(-edgeLength, -edgeLength,  edgeLength), glm::fvec3( 0.0f, -1.0f,  0.0f), glm::fvec2(0.0f, 0.0f)} };

	uint16_t Indices[NumIndices] = {
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

	StaticMeshCreateInfo createInfo;
	createInfo.NumVertices = NumVertices;
	createInfo.NumIndices = NumIndices;
	createInfo.Vertices = Vertices;
	createInfo.Indices = Indices;
	createInfo.IndexType = RHIIndexBufferType::IBT_16;

	CreateStaticMeshWithDefaultFilter(mesh, createInfo);
	return mesh;
}

StaticMesh_ptr StaticMeshGenerator::CreatePlane(float width /*= 1.0f*/, float height /*= 1.0f*/, size_t divisions /*= 1*/)
{
	std::vector<StaticMeshVertex> Vertices;
	std::vector<uint32_t> Indices;

	// no need to worry about the performance of this so much
	// it's a private function that should only be used in the editor
	// the resultant mesh should be stored anyways when building
	Vertices.resize(static_cast<size_t>(pow(divisions + 1, 2)));
	Indices.resize(static_cast<size_t>(pow(divisions, 2)) * 6);

	// TODO

	return nullptr;
}

void StaticMeshGenerator::CreateStaticMeshWithDefaultFilter(StaticMesh_ptr mesh, StaticMeshCreateInfo& info)
{
	mesh->NumVertices = info.NumVertices;
	mesh->NumIndices = info.NumIndices;
	mesh->NumTriangles = mesh->NumIndices / 3;
	assert(mesh->NumIndices == mesh->NumTriangles * 3);

	auto& RHICommandList = RHICommandListExecutor::GetImmediateCommandList();
	mesh->VertexBuffer = RHICommandList.CreateVertexBuffer(info.NumVertices,
														   sizeof(StaticMeshVertex),
														   RHIHardwareBufferUsage::HWBU_STATIC_WRITE_ONLY);
	mesh->IndexBuffer = RHICommandList.CreateIndexBuffer(info.NumIndices,
														 info.IndexType,
														 RHIHardwareBufferUsage::HWBU_STATIC_WRITE_ONLY);

	void* VerticesPtr = RHICommandList.LockVertexBuffer(mesh->VertexBuffer, 0, mesh->VertexBuffer->GetSize(), RHIResourceLockMode::RLM_WRITE_ONLY);
	void* IndicesPtr = RHICommandList.LockIndexBuffer(mesh->IndexBuffer, 0, mesh->IndexBuffer->GetSize(), RHIResourceLockMode::RLM_WRITE_ONLY);

	memcpy(VerticesPtr, info.Vertices, mesh->VertexBuffer->GetSize());
	memcpy(IndicesPtr, info.Indices, mesh->IndexBuffer->GetSize());
	RHICommandList.UnlockVertexBuffer(mesh->VertexBuffer);
	RHICommandList.UnlockIndexBuffer(mesh->IndexBuffer);
}