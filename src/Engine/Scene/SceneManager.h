
#ifndef ENGINE_SCENE_SCENEMANAGER_H
#define ENGINE_SCENE_SCENEMANAGER_H

#include "Scene.h"
#include "Actor.h"

class SceneManager
{
public:
	static SceneManager& GetInstance() {
		static SceneManager instance;
		return instance;
	}

	void SetCurrentScene(Scene_ptr scene) {
		mCurrentScene = scene;
	}
	void SetCurrentRenderScene(RScene_ptr scene) {
		mCurrentScene->mRenderScene = scene;
	}
	void SetCurrentPhysicsScene(PScene_ptr scene) {
		mCurrentScene->mPhysicsScene = scene;
	}

	void SetPlayer(Actor_ptr player) {
		mPlayer = player;
		mCurrentScene->mRenderScene->mMainCamera->SetParent(player);
	}

	void AddActor(Actor_ptr pActor);
	void RemoveActor(Actor_ptr pActor);
	std::vector<Actor_ptr> GetActors() const {
		if (mCurrentScene)
			return mCurrentScene->GetActors();
		return std::vector<Actor_ptr>();
	}

	void MovePlayer(Camera::MovementDirection dir) {
		mCurrentScene->mRenderScene->mMainCamera->Move(dir);
		mPlayer->Move(mCurrentScene->mRenderScene->mMainCamera->GetParentPosition());
	}
	void RotateCamera(double xDelta, double yDelta) {
		mCurrentScene->mRenderScene->mMainCamera->Rotate(xDelta, yDelta);
		//mCurrentScene->mRenderScene->mMainCamera->AddParentRotation();
	}
	void JumpPlayer() {
		if (mPlayer) {
			mPlayer->Jump();
		}
	}

	/// Render Functions
	void Render(RHICommandList& RHICmdList);

	
	/// Physics Functions
	void UpdatePhysics();

	void ShiftOrigin(glm::vec3 translation);

	bool Raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, PRaycastHit_ptr hit = nullptr);
	bool Raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, std::vector<PRaycastHit_ptr>& hits);
	bool Sweep(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, glm::vec3 direction, float maxDistance, PSweepHit_ptr hit = nullptr);
	bool Sweep(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, glm::vec3 direction, float maxDistance, std::vector<PSweepHit_ptr>& hit);
	bool Overlap(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, POverlapHit_ptr hit = nullptr);
	bool Overlap(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, std::vector<POverlapHit_ptr>& hit);

	//void SetCollisionRelationship(uint8_t categoryNum1, uint8_t categoryNum2, PScene::CollisionHandle collisionHandle, PScene::PairFlag pairFlag);


private:
	Scene_ptr mCurrentScene;
	
	Actor_ptr mPlayer;
};

#endif // ENGINE_SCENE_SCENEMANAGER_H