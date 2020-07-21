#pragma once
#include "../Core/StaticMesh.h"
#include "../Core/Transform.h"

class RenderActor;
typedef std::shared_ptr<RenderActor> RenderActor_ptr;

class RenderActor {
	friend class Actor;
public:
	RenderActor(StaticMesh_ptr mesh) : mMesh(mesh) {}

	Transform_ptr mTransform;
	StaticMesh_ptr mMesh;

protected:
	void SetTransform(Transform_ptr transform);
};