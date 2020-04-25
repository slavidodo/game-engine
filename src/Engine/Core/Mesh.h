
#ifndef ENGINE_CORE_MESH_H
#define ENGINE_CORE_MESH_H

#include "SubMesh.h"

class Mesh : public std::enable_shared_from_this<Mesh>
{
public:
	SubMesh_ptr CreateSubMesh();

	//OBJECT_ACCESSOR(VertexData_ptr, const VertexData_ptr, SharedVertexData);

private:
	// manual data assignment
	//VertexData_ptr mSharedVertexData = nullptr;

	std::vector<SubMesh_ptr> mSubMeshes;
};

#endif // ENGINE_CORE_MESH_H