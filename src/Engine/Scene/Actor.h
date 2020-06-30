#pragma once

#include "RActor.h"
#include "../Physics/PActor.h"

class Actor;
typedef std::shared_ptr<Actor> Actor_ptr;

class Actor {
public:
	Actor(RActor_ptr renderActor, PActor_ptr physicsActor) : mRenderActor(renderActor), mPhysicsActor(physicsActor) {}

	std::weak_ptr<PActor> mPhysicsActor;
	std::weak_ptr<RActor> mRenderActor;

	void Update();
};