#pragma once

#include "Engine/Core/Object.h"
#include "RenderActor.h"
#include "../Core/Texture.h"
#include "../Physics/PhysicsActor.h"
#include "../Utils.h"

class Actor;
typedef std::shared_ptr<Actor> Actor_ptr;

class Actor : public Object {
public:
	bool lighter = false;
	Actor(Transform_ptr transform, RenderActor_ptr renderActor, PhysicsActor_ptr physicsActor, int shaderMode = 0);

	std::weak_ptr<PhysicsActor> mPhysicsActor;
	std::weak_ptr<RenderActor> mRenderActor;
	int mShaderMode;
	void UpdateTransformsFromPhysics();

	void Move(glm::fvec3 displacement);
	void Jump();

	void SetTransform(Transform_ptr transform) {
		mTransform = transform;
	}
	Transform_ptr GetTransform() const {
		return mTransform;
	}
	void SetTexture(Texture_ptr texture) {
		mTexture = texture;
	}
	Texture_ptr GetTexture() const {
		return mTexture;
	}

protected:
	Transform_ptr mTransform;
	Texture_ptr mTexture;
};