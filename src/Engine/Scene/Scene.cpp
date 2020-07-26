
#include "pch.h"
#include "Scene.h"

Scene::Scene(RenderScene_ptr renderScene, PhysicsScene_ptr physicsScene) : mRenderScene(renderScene), mPhysicsScene(physicsScene), mDeltaTime(0.0f), mLastTime(0.0f) {}

float Scene::GetDeltaTime() {
	return mDeltaTime;
}

void Scene::AddActor(Actor_ptr actor) {
	if (PhysicsActor_ptr physicsActor = actor->mPhysicsActor.lock())
		mPhysicsScene->AddActor(physicsActor);
	if (RenderActor_ptr renderActor = actor->mRenderActor.lock())
		mRenderScene->AddActor(renderActor);

	mActors.push_back(actor);
}
void Scene::RemoveActor(Actor_ptr actor) {
	if (PhysicsActor_ptr physicsActor = actor->mPhysicsActor.lock())
		mPhysicsScene->RemoveActor(physicsActor);
	if (RenderActor_ptr renderActor = actor->mRenderActor.lock())
		mRenderScene->RemoveActor(renderActor);

	auto it = std::find(mActors.begin(), mActors.end(), actor);
	if (it != mActors.end()) mActors.erase(it);
}
std::vector<Actor_ptr> Scene::GetActors() const {
	return mActors;
}