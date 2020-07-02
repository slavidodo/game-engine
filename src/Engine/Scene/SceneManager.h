
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

	enum class MovementDirection { Forward, Backward, Upward, Downward, Right, Left };
	void MovePlayer(MovementDirection dir) {
		float movementSpeed = 0.05f;
		glm::fvec3 direction;
		switch (dir) {
		case MovementDirection::Forward:
			direction = glm::fvec3(0.0f, 0.0f, -1.0f);
			break;
		case MovementDirection::Backward:
			direction = glm::fvec3(0.0f, 0.0f, +1.0f);
			break;
		case MovementDirection::Upward:
			direction = glm::fvec3(0.0f, +1.0f, 0.0f);
			break;
		case MovementDirection::Downward:
			direction = glm::fvec3(0.0f, -1.0f, 0.0f);
			break;
		case MovementDirection::Right:
			direction = glm::fvec3(+1.0f, 0.0f, 0.0f);
			break;
		case MovementDirection::Left:
			direction = glm::fvec3(-1.0f, 0.0f, 0.0f);
			break;
		}
		mPlayer->Move(direction * movementSpeed);
	}
	void RotatePlayer(double xDelta, double yDelta) {
		//mPlayer->Rotate(xDelta, yDelta);
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