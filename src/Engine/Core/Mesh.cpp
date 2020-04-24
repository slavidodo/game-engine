
#include "pch.h"

#include "Mesh.h"

SubMesh_ptr Mesh::createSubMesh()
{
	auto subMesh = m_subMeshes.emplace_back();
	subMesh->parent(weak_from_this());

	return subMesh;
}