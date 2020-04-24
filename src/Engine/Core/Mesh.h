
#ifndef ENGINE_CORE_MESH_H
#define ENGINE_CORE_MESH_H

#include "SubMesh.h"

class Mesh : public std::enable_shared_from_this<Mesh>
{
public:
	SubMesh_ptr createSubMesh();

	//OBJECT_ACCESSOR(VertexData_ptr, const VertexData_ptr, sharedVertexData);

private:
	// manual data assignment
	//VertexData_ptr m_sharedVertexData = nullptr;

	std::vector<SubMesh_ptr> m_subMeshes;
};

#endif // ENGINE_CORE_MESH_H