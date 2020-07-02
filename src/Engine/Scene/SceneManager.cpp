
#include "pch.h"

#include "SceneManager.h"

void SceneManager::Render(RHICommandList& RHICmdList) {
	if (mCurrentScene)
		mCurrentScene->mRenderScene->mRenderFunction(RHICmdList);
}

void SceneManager::UpdatePhysics() {
	if (mCurrentScene)
		mCurrentScene->mPhysicsScene->mUpdateFunction();
}


void SceneManager::AddActor(Actor_ptr actor) {
	if (mCurrentScene) {
		mCurrentScene->AddActor(actor);
	}
		
}
void SceneManager::RemoveActor(Actor_ptr actor) {
	if (mCurrentScene) {
		mCurrentScene->RemoveActor(actor);
	}
}

void SceneManager::ShiftOrigin(glm::vec3 translation) {
	mCurrentScene->mPhysicsScene->ShiftOrigin(translation);
}

bool SceneManager::Raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, PRaycastHit_ptr hit) {
	return mCurrentScene->mPhysicsScene->Raycast(origin, direction, maxDistance, hit);
}
bool SceneManager::Raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, std::vector<PRaycastHit_ptr>& hits) {
	return mCurrentScene->mPhysicsScene->Raycast(origin, direction, maxDistance, hits);
}
bool SceneManager::Sweep(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, glm::vec3 direction, float maxDistance, PSweepHit_ptr hit) {
	return mCurrentScene->mPhysicsScene->Sweep(geometry, origin, rotation, direction, maxDistance, hit);
}
bool SceneManager::Sweep(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, glm::vec3 direction, float maxDistance, std::vector<PSweepHit_ptr>& hits) {
	return mCurrentScene->mPhysicsScene->Sweep(geometry, origin, rotation, direction, maxDistance, hits);
}
bool SceneManager::Overlap(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, POverlapHit_ptr hit) {
	return mCurrentScene->mPhysicsScene->Overlap(geometry, origin, rotation, hit);
}
bool SceneManager::Overlap(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, std::vector<POverlapHit_ptr>& hits) {
	return mCurrentScene->mPhysicsScene->Overlap(geometry, origin, rotation, hits);
}

//void SceneManager::SetCollisionRelationship(uint8_t categoryNum1, uint8_t categoryNum2, Scene::CollisionHandle collisionHandle, Scene::PairFlag pairFlag) {
//	mCurrentScene->mPhysicsScene->SetCollisionRelationship(categoryNum1, categoryNum2, collisionHandle, pairFlag);
//}