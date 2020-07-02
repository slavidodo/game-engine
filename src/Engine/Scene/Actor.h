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
	
	void Move(glm::fvec3 displacement) {
		PDynamicActor_ptr actor = std::static_pointer_cast<PDynamicActor>(mPhysicsActor.lock());
		actor->SetPosition(actor->GetPosition() + displacement);
		UpdateTransforms();
	}
	void Rotate(double xDelta, double yDelta) {
		std::cout << "rotating player\n";
		PDynamicActor_ptr actor = std::static_pointer_cast<PDynamicActor>(mPhysicsActor.lock());
		actor->ApplyTorque(glm::fvec3(0.0f, xDelta * 0.1f, 0.0f));
		UpdateTransforms();
	}
	void Jump() {
		PDynamicActor_ptr actor = std::static_pointer_cast<PDynamicActor>(mPhysicsActor.lock());
		actor->ApplyImpulse(glm::vec3(0.0f, 0.2f, 0.0f));
		UpdateTransforms();
	}

	void SetTransform(Transform_ptr transform) {
		mTransform = transform;
	}
	Transform_ptr GetTransform() const {
		return mTransform;
	}

protected:
	Transform_ptr mTransform;
};