
#include "pch.h"

#include "StaticMesh.h"

#include "../Platform/Window.h"

void StaticMesh::Release()
{
	// TODO: is this safe?
	VertexBuffer.SafeRelease();
	IndexBuffer.SafeRelease();
	NumVertices = 0;
	NumIndices = 0;
	NumTriangles = 0;
}