#include "pch.h"
#include "Actor.h"

void Actor::UpdateTransforms() {
	RActor_ptr renderActor = mRenderActor.lock();
	PActor_ptr physicsActor = mPhysicsActor.lock();
	if (renderActor && physicsActor) {
		mTransform = physicsActor->GetTransform();
		renderActor->Update(mTransform);
	}
}