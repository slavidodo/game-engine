#pragma once
#include "../Core/StaticMesh.h"
#include "../Core/Transform.h"

class RActor;
typedef std::shared_ptr<RActor> RActor_ptr;

class RActor {
public:
	RActor(Transform_ptr transform, StaticMesh_ptr mesh) : mTransform(transform), mMesh(mesh) {}

	Transform_ptr mTransform;
	StaticMesh_ptr mMesh;

	void Update(Transform_ptr transform);
};