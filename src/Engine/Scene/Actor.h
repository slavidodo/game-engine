#pragma once

#include "Engine/Core/Object.h"
#include "RenderActor.h"
#include "../Physics/PhysicsActor.h"
#include "../Utils.h"

class Actor;
typedef std::shared_ptr<Actor> Actor_ptr;

class Actor : public Object {
public:
	Actor(Transform_ptr transform, RenderActor_ptr renderActor, PhysicsActor_ptr physicsActor);

	std::weak_ptr<PhysicsActor> mPhysicsActor;
	std::weak_ptr<RenderActor> mRenderActor;

	void UpdateTransformsFromPhysics();
	
	void Move(glm::fvec3 displacement);
	void Jump();

	void SetTransform(Transform_ptr transform) {
		mTransform = transform;
	}
	Transform_ptr GetTransform() const {
		return mTransform;
	}

protected:
	Transform_ptr mTransform;
	
};