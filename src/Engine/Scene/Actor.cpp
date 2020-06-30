#include "pch.h"
#include "Actor.h"

void Actor::Update() {
	RActor_ptr renderActor = mRenderActor.lock();
	PActor_ptr physicsActor = mPhysicsActor.lock();
	if (renderActor && physicsActor)
		renderActor->Update(physicsActor->GetTransform());
}