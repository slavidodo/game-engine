
#include "pch.h"

#include "Mesh.h"

SubMesh_ptr Mesh::createSubMesh()
{
	auto subMesh = mSubMeshes.emplace_back();
	subMesh->SetParent(weak_from_this());

	return subMesh;
}