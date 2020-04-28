
#include "pch.h"

#include "StaticMesh.h"

#include "../Platform/Window.h"

void StaticMesh::Release()
{
	// TODO: is this safe?
	mVertexBuffer.reset();
	mIndexBuffer.reset();
	mNumVertices = 0;
	mNumIndices = 0;
}