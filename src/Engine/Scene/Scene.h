#pragma once
#include "PhysicsScene.h"
#include "RenderScene.h"
#include "Actor.h"

class Scene;
typedef std::shared_ptr<Scene> Scene_ptr;

class Scene {
public:
	Scene(RenderScene_ptr renderScene, PhysicsScene_ptr physicsScene);

	float GetDeltaTime();

	std::vector<Actor_ptr> GetActors() const;
	void AddActor(Actor_ptr actor);
	void RemoveActor(Actor_ptr actor);

	PhysicsScene_ptr mPhysicsScene;
	RenderScene_ptr mRenderScene;
	
protected:
	double mDeltaTime, mLastTime;

	std::vector<Actor_ptr> mActors;
};