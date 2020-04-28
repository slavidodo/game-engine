
#ifndef ENGINE_CORE_MESH_H
#define ENGINE_CORE_MESH_H

#include "../RHI/RHIResource.h"

class StaticMesh;
typedef std::shared_ptr<StaticMesh> StaticMesh_ptr;

struct StaticVertexFilter
{
	StaticVertexFilter(glm::fvec3 position, glm::fvec3 normal, glm::fvec2 uv)
		: position(position), normal(normal), uv(uv) {}

	glm::fvec3 position;
	glm::fvec3 normal;
	glm::fvec2 uv;
};

class StaticMesh : public std::enable_shared_from_this<StaticMesh>
{
public:
	virtual ~StaticMesh() {
		Release();
	}

	void Release();
private:
	friend class StaticMeshGenerator;
	friend class RHIContext;

	RHIVertexBuffer_ptr mVertexBuffer;
	RHIIndexBuffer_ptr mIndexBuffer;

	size_t mNumVertices;
	size_t mNumIndices;
	size_t mNumTriangles;
};

#endif // ENGINE_CORE_MESH_H