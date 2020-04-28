
#include "pch.h"

#include "StaticMeshGenerator.h"

#include "Platform/Window.h"

StaticMesh_ptr StaticMeshGenerator::CreateCube(float edgeLength /* = 1.0f */)
{
	auto mesh = std::make_shared<StaticMesh>();

	// initial information
	const size_t NumVertices = 4 * 6;
	const size_t NumIndices = 2 * 6 * 3;

	StaticVertexFilter Vertices[NumVertices] = {
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


void StaticMeshGenerator::CreateStaticMeshWithDefaultFilter(StaticMesh_ptr mesh, StaticMeshCreateInfo& info)
{
	RHIContext* DynamicRHI = g_window.GetContext();

	mesh->mVertexBuffer = DynamicRHI->CreateVertexBuffer(info.NumVertices,
																	  sizeof(StaticVertexFilter),
																	  RHIHardwareBufferUsage::HWBU_STATIC_WRITE_ONLY);
	mesh->mIndexBuffer = DynamicRHI->CreateIndexBuffer(info.NumIndices,
																   info.IndexType,
																   RHIHardwareBufferUsage::HWBU_STATIC_WRITE_ONLY);

	void* pVoidVertices = DynamicRHI->LockVertexBuffer(mesh->mVertexBuffer, 0, mesh->mVertexBuffer->GetSize(), RHIResourceLockMode::RLM_WRITE_ONLY);
	void* pVoidIndices = DynamicRHI->LockIndexBuffer(mesh->mIndexBuffer, 0, mesh->mIndexBuffer->GetSize(), RHIResourceLockMode::RLM_WRITE_ONLY);
	std::memcpy(pVoidVertices, (void*)info.Vertices, mesh->mVertexBuffer->GetSize());
	std::memcpy(pVoidIndices, (void*)info.Indices, mesh->mIndexBuffer->GetSize());
	DynamicRHI->UnlockVertexBuffer(mesh->mVertexBuffer);
	DynamicRHI->UnlockIndexBuffer(mesh->mIndexBuffer);

	mesh->mNumVertices = info.NumVertices;
	mesh->mNumIndices = info.NumIndices;
	mesh->mNumTriangles = mesh->mNumIndices / 3;
}