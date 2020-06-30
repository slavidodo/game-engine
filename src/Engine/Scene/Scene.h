#pragma once
#include "PScene.h"
#include "RScene.h"
#include "Actor.h"

class Scene;
typedef std::shared_ptr<Scene> Scene_ptr;

class Scene {
public:
	Scene(RScene_ptr renderScene, PScene_ptr physicsScene) : mRenderScene(renderScene), mPhysicsScene(physicsScene) {}

	std::vector<Actor_ptr> GetActors() const {
		return mActors;
	}
	void AddActor(Actor_ptr actor) {
		mActors.push_back(actor);
	}
	void RemoveActor(Actor_ptr actor) {
		auto it = std::find(mActors.begin(), mActors.end(), actor);
		if (it != mActors.end()) mActors.erase(it);
	}

	PScene_ptr mPhysicsScene;
	RScene_ptr mRenderScene;
	std::vector<Actor_ptr> mActors;
};