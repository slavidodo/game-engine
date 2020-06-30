
#include "pch.h"
#include "PSceneManager.h"

void PSceneManager::SetCurrentScene(PScene_ptr scene) { // completely destroys the previous scene
	mCurrentScene = scene;
}

void PSceneManager::AddActor(PActor_ptr pActor) {
	mCurrentScene->AddActor(pActor);
}
void PSceneManager::RemoveActor(PActor_ptr pActor) {
	mCurrentScene->RemoveActor(pActor);
}

void PSceneManager::Update(float deltaTime) {
	mCurrentScene->Update(deltaTime);
}
void PSceneManager::ApplyUpdateResults(bool bSleep) {
	mCurrentScene->ApplyUpdateResults(bSleep);

	std::list<PActor_ptr> sceneActors = mCurrentScene->GetActors();
	for (PActor_ptr actor : sceneActors) {
		Transform_ptr physicsTransform = actor->GetTransform();
		Transform_ptr renderTransform = actor->GetRenderTransform();
		if (renderTransform && physicsTransform) {
			renderTransform->SetTranslation(physicsTransform->GetTranslation());
			renderTransform->SetRotation(physicsTransform->GetRotation());
		}
	}
}

void PSceneManager::ShiftOrigin(glm::vec3 translation) {
	mCurrentScene->ShiftOrigin(translation);
}

bool PSceneManager::Raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, PRaycastHit_ptr hit) {
	return mCurrentScene->Raycast(origin, direction, maxDistance, hit);
}
bool PSceneManager::Raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, std::vector<PRaycastHit_ptr>& hits) {
	return mCurrentScene->Raycast(origin, direction, maxDistance, hits);
}
bool PSceneManager::Sweep(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, glm::vec3 direction, float maxDistance, PSweepHit_ptr hit) {
	return mCurrentScene->Sweep(geometry, origin, rotation, direction, maxDistance, hit);
}
bool PSceneManager::Sweep(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, glm::vec3 direction, float maxDistance, std::vector<PSweepHit_ptr>& hits) {
	return mCurrentScene->Sweep(geometry, origin, rotation, direction, maxDistance, hits);
}
bool PSceneManager::Overlap(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, POverlapHit_ptr hit) {
	return mCurrentScene->Overlap(geometry, origin, rotation, hit);
}
bool PSceneManager::Overlap(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, std::vector<POverlapHit_ptr>& hits) {
	return mCurrentScene->Overlap(geometry, origin, rotation, hits);
}

void PSceneManager::SetCollisionRelationship(uint8_t categoryNum1, uint8_t categoryNum2, PScene::CollisionHandle collisionHandle, PScene::PairFlag pairFlag) {
	mCurrentScene->SetCollisionRelationship(categoryNum1, categoryNum2, collisionHandle, pairFlag);
}