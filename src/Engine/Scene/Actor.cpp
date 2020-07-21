#include "pch.h"
#include "Actor.h"
#include "SceneManager.h"

Actor::Actor(Transform_ptr transform, RenderActor_ptr renderActor, PhysicsActor_ptr physicsActor) : mTransform(transform), mRenderActor(renderActor), mPhysicsActor(physicsActor) {
	if (renderActor)
		renderActor->SetTransform(mTransform);
}

void Actor::UpdateTransformsFromPhysics() {
	PhysicsActor_ptr physicsActor = mPhysicsActor.lock();
	if (physicsActor) {
		mTransform->SetTranslation(physicsActor->GetTransform()->GetTranslation());
		//std::cout << "Position: " << mTransform->GetTranslation().x << " " << mTransform->GetTranslation().y << " " << mTransform->GetTranslation().z << std::endl;
	}
}

void Actor::Move(glm::fvec3 displacement) {
	mTransform->SetTranslation(mTransform->GetTranslation() + displacement);
	if (PhysicsActor_ptr physicsActor = mPhysicsActor.lock()) {
		PDynamicActor_ptr dynamicActor = std::static_pointer_cast<PDynamicActor>(physicsActor);
		dynamicActor->SetPosition(GetTransform()->GetTranslation());
	}
}
void Actor::Jump() {
	if (PhysicsActor_ptr physicsActor = mPhysicsActor.lock()) {
		Transform_ptr playerTransform = physicsActor->GetTransform();
		glm::fvec3 raycastOrigin = glm::fvec3(playerTransform->GetTranslation());

		float halfHeight = 0.0f;
		std::vector<PRaycastHit_ptr> hits;
		if (PhysicsActor_ptr physicsActor = mPhysicsActor.lock()) 
			halfHeight = physicsActor->GetTransform()->GetScale().y / 2.0f;
		
		SceneManager::GetInstance().Raycast(raycastOrigin, glm::fvec3(0.0f, -1.0f, 0.0f), 0.01f + halfHeight, hits);
		bool bOnGround = hits.size() > 1;
		if (bOnGround) {
			PDynamicActor_ptr actor = std::static_pointer_cast<PDynamicActor>(physicsActor);
			actor->ApplyImpulse(glm::vec3(0.0f, 0.2f, 0.0f));
			UpdateTransformsFromPhysics();
		}
	}
}