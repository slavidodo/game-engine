#pragma once

#include "Engine/Core/Object.h"
#include "RActor.h"
#include "../Physics/PActor.h"

class Actor;
typedef std::shared_ptr<Actor> Actor_ptr;

class Actor : public Object {
public:
	Actor(RActor_ptr renderActor, PActor_ptr physicsActor) : mRenderActor(renderActor), mPhysicsActor(physicsActor) {
		mTransform = renderActor->mTransform;
	}

	std::weak_ptr<PActor> mPhysicsActor;
	std::weak_ptr<RActor> mRenderActor;

	void UpdateTransforms();
	
	void Move(glm::fvec3 newPosition);
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