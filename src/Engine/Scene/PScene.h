//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef ENGINE_PHYSICS_SCENE_H
#define ENGINE_PHYSICS_SCENE_H

#include "physx/PxPhysicsAPI.h"
#include "../Scene/PScene.h"
#include "../Physics/PSceneQuery.h"

class PScene;
typedef std::shared_ptr<PScene> PScene_ptr;

physx::PxFilterFlags FilterShaderFunction(physx::PxFilterObjectAttributes attribs1, physx::PxFilterData filterData1, physx::PxFilterObjectAttributes attribs2, physx::PxFilterData filterData2, physx::PxPairFlags& pairFlags, const void* constBlock, uint32_t constBlockSize);

struct PSceneDescriptor 
{
public:
	PSceneDescriptor();
	~PSceneDescriptor();

	physx::PxSceneDesc* GetSdkDescriptor() const;

	void SetGravityForce(glm::vec3 force = glm::vec3(0.0f, -9.8f, 0.0f));
	void SetCpuDispatcher(physx::PxCpuDispatcher* cpuDispatcher);
	void setFilterShader(physx::PxSimulationFilterShader filterShader);

private:
	physx::PxSceneDesc* mDescriptor;
};

class PScene 
{
public:
	PScene(physx::PxScene* pScene);
	~PScene();

	static PScene_ptr CreateScene(const PSceneDescriptor& pPSceneDesc);

	void AddActor(PActor_ptr pActor);
	void RemoveActor(PActor_ptr pActor);
	std::list<PActor_ptr> GetActors() const {
		return mActors;
	}

	void Update(float dt);

	void ShiftOrigin(glm::vec3 translation);

	bool Raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, PRaycastHit_ptr hit = nullptr);
	bool Raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, std::vector<PRaycastHit_ptr>& hits);
	bool Sweep(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, glm::vec3 direction, float maxDistance, PSweepHit_ptr hit = nullptr);
	bool Sweep(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, glm::vec3 direction, float maxDistance, std::vector<PSweepHit_ptr>& hit);
	bool Overlap(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, POverlapHit_ptr hit = nullptr);
	bool Overlap(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, std::vector<POverlapHit_ptr>& hit);

	enum class CollisionHandle {
		Default = physx::PxPairFlag::eCONTACT_DEFAULT,
		Trigger = physx::PxPairFlag::eTRIGGER_DEFAULT,
	};

	enum class PairFlag {
		Default = physx::PxFilterFlag::eDEFAULT, // processes collision, then ignores this pair until it falls out then in collision again, or until any filter relevant data changes.
		Kill = physx::PxFilterFlag::eKILL, // ignores collision, then ignores this pair until it falls out then in collision again.
		Suppress = physx::PxFilterFlag::eSUPPRESS, // ignores collision, then ignores this pair until the pair falls out then in collision again, or until any filter relevant data changes.
		Callback = physx::PxFilterFlag::eCALLBACK, // invokes the filter callback PairFound().
		Notify = physx::PxFilterFlag::eNOTIFY, // invokes the filter callback PairFound() when the pair falls in collision, and PairLost() when it falls out of collision.
	};

	void SetCollisionRelationship(uint8_t categoryNum1, uint8_t categoryNum2, CollisionHandle collisionHandle, PairFlag pairFlag);

	// Joint visualization doesn't work for some reason?
	void SetJointVisualization(bool value);

	std::function<void()> mUpdateFunction;

private:
	physx::PxScene* mScene = nullptr;

	std::list<PActor_ptr> mActors;

	std::pair<physx::PxPairFlag::Enum, physx::PxFilterFlag::Enum> mCollisionMatrix[128][128];
};

#endif