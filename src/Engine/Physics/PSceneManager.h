#ifndef ENGINE_PHYSICS_SCENE_MANAGER_H
#define ENGINE_PHYSICS_SCENE_MANAGER_H

#include "Engine/Physics/PScene.h"

class PSceneManager 
{
public:
	static PSceneManager& GetInstance() {
		static PSceneManager instance;
		return instance;
	}

	void SetCurrentScene(PScene_ptr scene);
	
	void AddActor(PActor_ptr pActor);
	void RemoveActor(PActor_ptr pActor);

	void Update(float deltaTime);
	void ApplyUpdateResults(bool bSleep);

	void ShiftOrigin(glm::vec3 translation);

	bool Raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, PRaycastHit_ptr hit = nullptr);
	bool Raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, std::vector<PRaycastHit_ptr>& hits);
	bool Sweep(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, glm::vec3 direction, float maxDistance, PSweepHit_ptr hit = nullptr);
	bool Sweep(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, glm::vec3 direction, float maxDistance, std::vector<PSweepHit_ptr>& hit);
	bool Overlap(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, POverlapHit_ptr hit = nullptr);
	bool Overlap(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, std::vector<POverlapHit_ptr>& hit);

	void SetCollisionRelationship(uint8_t categoryNum1, uint8_t categoryNum2, PScene::CollisionHandle collisionHandle, PScene::PairFlag pairFlag);

private:
	PScene_ptr mCurrentScene;
};

#endif