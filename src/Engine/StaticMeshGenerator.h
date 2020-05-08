
#ifndef ENGINE_STATICMESHGENERATOR_H
#define ENGINE_STATICMESHGENERATOR_H

#include "Core/StaticMesh.h"

// CREATE PREFABS

struct StaticMeshCreateInfo
{
	void* Vertices;
	void* Indices;
	size_t NumVertices;
	size_t NumIndices;
	RHIIndexBufferType IndexType;
};

class StaticMeshGenerator
{
public:
	static StaticMesh_ptr CreateTriangle(float edgeLength = 1.0f);
	static StaticMesh_ptr CreateQuad(float edgeLength = 1.0f);
	static StaticMesh_ptr CreateCube(float edgeLength = 1.0f);
	static StaticMesh_ptr CreateSphere(float radius = 1.0f);
	static StaticMesh_ptr CreatePlane(float width = 1.0f, float height = 1.0f, size_t division = 1U);

private:
	// TODO: Add more flexibity to choose vertex size, supplied information, ..
	// for now we stick to a pos/uv/uv and it will work pretty much well for everything
	// rememeber when loading with assimp to check normals, if not present
	// set them to a defautl value
	// we could also add tangets to be included in static meshes by default
	static void CreateStaticMeshWithDefaultFilter(StaticMesh_ptr mesh, StaticMeshCreateInfo& info);
};

#endif // ENGINE_STATICMESHGENERATOR_H