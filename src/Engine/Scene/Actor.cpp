#include "pch.h"
#include "Actor.h"
#include "SceneManager.h"

void Actor::UpdateTransforms() {
	RActor_ptr renderActor = mRenderActor.lock();
	PActor_ptr physicsActor = mPhysicsActor.lock();
	if (renderActor && physicsActor) {
		mTransform = physicsActor->GetTransform();
		renderActor->Update(mTransform);
	}
}

void Actor::Move(glm::fvec3 newPosition) {
	PDynamicActor_ptr actor = std::static_pointer_cast<PDynamicActor>(mPhysicsActor.lock());
	actor->SetPosition(newPosition);
	UpdateTransforms();
}
void Actor::Jump() {
	glm::fvec3 raycastOrigin = glm::fvec3(mTransform->GetTranslation());
	raycastOrigin.y -= mTransform->GetScale().y / 2.0f + 0.000001f;
	if (SceneManager::GetInstance().Raycast(raycastOrigin, glm::fvec3(0.0f, -1.0f, 0.0f), 0.1f)) {
		PDynamicActor_ptr actor = std::static_pointer_cast<PDynamicActor>(mPhysicsActor.lock());
		actor->ApplyImpulse(glm::vec3(0.0f, 0.25f, 0.0f));
		UpdateTransforms();
	}
	else {
		/*PDynamicActor_ptr actor = std::static_pointer_cast<PDynamicActor>(mPhysicsActor.lock());
		actor->ApplyImpulse(glm::vec3(0.0f, 0.5f, 0.0f));
		UpdateTransforms();*/
	}
}