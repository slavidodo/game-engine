
#ifndef ENGINE_CORE_MESH_H
#define ENGINE_CORE_MESH_H

#include "../RHI/RHIResource.h"

class StaticMesh;
typedef std::shared_ptr<StaticMesh> StaticMesh_ptr;

struct StaticMeshVertex
{
	StaticMeshVertex()
		: position(0), normal(0), uv(0) {}

	StaticMeshVertex(glm::fvec3 position, glm::fvec3 normal, glm::fvec2 uv)
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
	
	// todo: should wrap rhi into a buffer object
	// so we don't actually have to work with RHI at all
	RHIVertexBuffer_ptr VertexBuffer;
	RHIIndexBuffer_ptr IndexBuffer;
	size_t NumVertices;
	size_t NumIndices;
	size_t NumTriangles;

	void Release();

private:
	friend class StaticMeshGenerator;
	friend class RHIContext;

};

#endif // ENGINE_CORE_MESH_H