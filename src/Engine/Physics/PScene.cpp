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

#include "Pch.h"
#include "PhysicsEngine.h"

#pragma region Scene Core Functions
PScene::PScene(physx::PxScene* pScene) : m_pScene(pScene) {
	for (int i = 0; i < 128; i++) m_collisionMatrix[0][i] = m_collisionMatrix[i][0] = { 
		physx::PxPairFlag::eCONTACT_DEFAULT,
		physx::PxFilterFlag::eDEFAULT
	};
	m_pScene->setFilterShaderData(m_collisionMatrix, sizeof(uint8_t) * 128 * 128);
}
PScene::~PScene() {
	if (!m_pScene) return;
	
	PAlignedAllocator::deallocate(m_pScene->userData);
	m_pScene->release();
}

PScene_ptr PScene::createScene(const PSceneDescriptor& pSceneDesc) {
	physx::PxScene* pScene = g_pPhysicsEngine->m_pPhysics->createScene(*pSceneDesc.getSdkDescriptor());
	if (!pScene) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating scene", __FILE__, __LINE__);
		return nullptr;
	}

	PScene_ptr scene = std::make_shared<PScene>(pScene);
	scene->m_pScene->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PScene>>()) std::weak_ptr<PScene>(scene);

	return std::move(scene);
}

void PScene::addActor(PActor_ptr actor) {
	m_pScene->addActor(*actor->getSdkActor());
	m_pActors.push_back(actor);
}
void PScene::removeActor(PActor_ptr actor) {
	auto it = std::find(m_pActors.begin(), m_pActors.end(), actor);
	if (it == m_pActors.end()) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Trying to remove an actor that is not in this scene", __FILE__, __LINE__);
		return;
	}

	m_pScene->removeActor(*actor->getSdkActor());
	m_pActors.erase(it);
}

void PScene::update(float dt) {
	m_pScene->simulate(dt);
}
void PScene::applyUpdateResults(bool bSleep) {
	m_pScene->fetchResults(bSleep);
}

void PScene::shiftOrigin(glm::vec3 translation) {
	m_pScene->shiftOrigin(physx::PxVec3(translation.x, translation.y, translation.z));
}

bool PScene::raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, PRaycastHit_ptr hit) {
	physx::PxRaycastBuffer hitsBuffer;
	bool bHit = m_pScene->raycast(physx::PxVec3(origin.x, origin.y, origin.z), physx::PxVec3(direction.x, direction.y, direction.z), maxDistance, hitsBuffer);
	if (!bHit) return false;
	
	if (hit) hit->createHit(&hitsBuffer.block);
	return true;
}
bool PScene::raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, std::vector<PRaycastHit_ptr>& hits) {
	const uint32_t bufferSize = 128;
	physx::PxRaycastHit buffer[bufferSize];
	physx::PxRaycastBuffer hitsBuffer(buffer, bufferSize);
	bool bHit = m_pScene->raycast(physx::PxVec3(origin.x, origin.y, origin.z), physx::PxVec3(direction.x, direction.y, direction.z), maxDistance, hitsBuffer);
	if (!bHit) return false;

	for (uint32_t i = 0; i < hitsBuffer.nbTouches; i++) {
		hits.push_back(std::make_shared<PRaycastHit>());
		hits.back()->createHit(&hitsBuffer.touches[i]);
	}
	return true;
}
bool PScene::sweep(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, glm::vec3 direction, float maxDistance, PSweepHit_ptr hit) {
	physx::PxSweepBuffer hitsBuffer;
	physx::PxTransform transform = physx::PxTransform(physx::PxVec3(origin.x, origin.y, origin.z), physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w));
	bool bHit = m_pScene->sweep(*geometry->getSdkGeometry(), transform, physx::PxVec3(direction.x, direction.y, direction.z), maxDistance, hitsBuffer);
	if (!bHit) return false;
	
	if (hit) hit->createHit(&hitsBuffer.block);
	return true;
}
bool PScene::sweep(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, glm::vec3 direction, float maxDistance, std::vector<PSweepHit_ptr>& hits) {
	const uint32_t bufferSize = 128;
	physx::PxSweepHit buffer[bufferSize];
	physx::PxSweepBuffer hitsBuffer(buffer, bufferSize);
	physx::PxTransform transform = physx::PxTransform(physx::PxVec3(origin.x, origin.y, origin.z), physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w));
	bool bHit = m_pScene->sweep(*geometry->getSdkGeometry(), transform, physx::PxVec3(direction.x, direction.y, direction.z), maxDistance, hitsBuffer);
	if (!bHit) return false;

	for (uint32_t i = 0; i < hitsBuffer.nbTouches; i++) {
		hits.push_back(std::make_shared<PSweepHit>());
		hits.back()->createHit(&hitsBuffer.touches[i]);
	}
	return true;
}
bool PScene::overlap(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, POverlapHit_ptr hit) {
	physx::PxOverlapBuffer hits;
	physx::PxTransform transform = physx::PxTransform(physx::PxVec3(origin.x, origin.y, origin.z), physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w));
	physx::PxQueryFilterData filterData = physx::PxQueryFilterData(physx::PxQueryFlags(physx::PxQueryFlag::eANY_HIT));
	
	bool bHit = m_pScene->overlap(*geometry->getSdkGeometry(), transform, hits, filterData);
	if (!bHit) return false;

	hit->createHit(new (PAlignedAllocator::allocate<physx::PxOverlapHit>()) physx::PxOverlapHit(hits.block));
	return true;
}
bool PScene::overlap(PGeometry_ptr geometry, glm::vec3 origin, glm::quat rotation, std::vector<POverlapHit_ptr>& hits) {
	const uint32_t bufferSize = 128;
	physx::PxOverlapHit buffer[bufferSize];
	physx::PxOverlapBuffer hitsBuffer(buffer, bufferSize);
	physx::PxTransform transform = physx::PxTransform(physx::PxVec3(origin.x, origin.y, origin.z), physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w));
	bool bHit = m_pScene->overlap(*geometry->getSdkGeometry(), transform, hitsBuffer);
	if (!bHit) return false;

	for (uint32_t i = 0; i < hitsBuffer.nbTouches; i++) {
		hits.push_back(std::make_shared<POverlapHit>());
		hits.back()->createHit(&hitsBuffer.touches[i]);
	}
	return true;
}

void PScene::setCollisionRelationship(uint8_t categoryNum1, uint8_t categoryNum2, CollisionHandle collisionHandle, PairFlag pairFlag) {
	m_collisionMatrix[categoryNum1][categoryNum2] = m_collisionMatrix[categoryNum2][categoryNum1] = {
		static_cast<physx::PxPairFlag::Enum>(collisionHandle),
		static_cast<physx::PxFilterFlag::Enum>(pairFlag)
	};
	m_pScene->setFilterShaderData(m_collisionMatrix, sizeof(uint8_t) * 128 * 128);
}

void PScene::setJointVisualization(bool value) {
	float val = (value ? 1.0f : 0.0f);
	
	bool success = m_pScene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, val);
	success &= m_pScene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, val);

	if (!success) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "visualization parameter value out of range", __FILE__, __LINE__);
		return;
	}
}
#pragma endregion
#pragma region Descriptor Functions
PSceneDescriptor::PSceneDescriptor() {
	m_pDescriptor = new (PAlignedAllocator::allocate<physx::PxSceneDesc>()) physx::PxSceneDesc(g_pPhysicsEngine->m_pPhysics->getTolerancesScale());
	setFilterShader(FilterShaderFunction);
	setCpuDispatcher(physx::PxDefaultCpuDispatcherCreate(0));
}
PSceneDescriptor::~PSceneDescriptor() {
	PAlignedAllocator::deallocate(m_pDescriptor);
}

physx::PxSceneDesc* PSceneDescriptor::getSdkDescriptor() const {
	return m_pDescriptor;
}

void PSceneDescriptor::setGravityForce(float x, float y, float z) {
	m_pDescriptor->gravity = physx::PxVec3(x, y, z);
}
void PSceneDescriptor::setFilterShader(physx::PxSimulationFilterShader filterShader) {
	m_pDescriptor->filterShader = filterShader;
}
void PSceneDescriptor::setCpuDispatcher(physx::PxCpuDispatcher* cpuDispatcher) {
	m_pDescriptor->cpuDispatcher = cpuDispatcher;
	if (!m_pDescriptor->cpuDispatcher) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error provided CPU Dispatcher invalid", __FILE__, __LINE__);
		return;
	}
}

/// <param name = 'attribs1'> type of first actor (static, dynamic, kinematic, trigger, etc.) </param>
/// <param name = 'filterData1'> 128 bits of user defined data found in the shape of the first actor </param>
/// <param name = 'attribs2'> type of second actor (static, dynamic, kinematic, trigger, etc.) </param>
/// <param name = 'filterData2'> 128 bits of user defined data found in the shape of the first actor </param>
/// <param name = 'pairFlags'> output param that determines how the actors will be handled physically </param>
/// <param name = 'constBlock'> custom global scene info </param>
/// <returns> determines when this function will be called again for this specific pair </returns>
#pragma endregion

physx::PxFilterFlags FilterShaderFunction(physx::PxFilterObjectAttributes attribs1, physx::PxFilterData filterData1, physx::PxFilterObjectAttributes attribs2, physx::PxFilterData filterData2, physx::PxPairFlags& pairFlags, const void* constBlock, uint32_t constBlockSize) {
	/// Filter trigger objects
	if (physx::PxFilterObjectIsTrigger(attribs1) || physx::PxFilterObjectIsTrigger(attribs2)) {
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	/// Find the collision categories of the two shapes
	auto findCategoryNum = [](physx::PxFilterData filterData) {
		uint8_t categoryNum = 0;
		uint32_t words[4] = {
			filterData.word0,
			filterData.word1,
			filterData.word2,
			filterData.word3
		};
		for (uint8_t i = 0; i < 4; i++) {
			if (!words[i]) continue;

			categoryNum = i * 32;
			while (words[i]) { words[i] >>= 1; categoryNum++; }
			break;
		}
		return categoryNum - 1;
	};
	uint8_t categoryNum1 = findCategoryNum(filterData1);
	uint8_t categoryNum2 = findCategoryNum(filterData2);

	/// Check the collision matrix for desired way of handling
	#define filterChoices std::pair<physx::PxPairFlag::Enum, physx::PxFilterFlag::Enum>
	filterChoices(&collisionMatrix)[128][128] = *reinterpret_cast<filterChoices(*)[128][128]>(const_cast<void*>(constBlock));
	
	pairFlags = collisionMatrix[categoryNum1][categoryNum2].first;
	return collisionMatrix[categoryNum1][categoryNum2].second;
}