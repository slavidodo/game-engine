
#include "pch.h"

#include "Mesh.h"

SubMesh_ptr Mesh::CreateSubMesh()
{
	SubMesh_ptr subMesh = mSubMeshes.emplace_back();
	subMesh->SetParent(weak_from_this());

	return subMesh;
}