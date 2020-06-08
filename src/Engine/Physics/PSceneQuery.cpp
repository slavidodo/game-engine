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

#pragma region Query Hit Functions
PQueryHit::~PQueryHit() {
	PAlignedAllocator::deallocate(m_pQueryHit);
}

PActor_ptr PQueryHit::getActor() const {
	physx::PxRigidActor* sdkActor = m_pQueryHit->actor;
	std::weak_ptr<PActor> wpActor = *static_cast<std::weak_ptr<PActor>*>(sdkActor->userData);
	if (PActor_ptr spActor = wpActor.lock())
		return std::move(spActor);
	return nullptr;
}
PCollider_ptr PQueryHit::getCollider() const {
	physx::PxShape* sdkCollider = m_pQueryHit->shape;
	std::weak_ptr<PCollider> wpCollider = *static_cast<std::weak_ptr<PCollider>*>(sdkCollider->userData);
	if (PCollider_ptr spActor = wpCollider.lock())
		return std::move(spActor);
	return nullptr;
}
#pragma endregion
#pragma region Point Hit Functions
PPointHit::~PPointHit() {
	PAlignedAllocator::deallocate(m_pPointHit);
}

PActor_ptr PPointHit::getActor() const {
	physx::PxRigidActor* sdkActor = m_pPointHit->actor;
	std::weak_ptr<PActor> wpActor = *static_cast<std::weak_ptr<PActor>*>(sdkActor->userData);
	
	if (PActor_ptr spActor = wpActor.lock())
		return std::move(spActor);
	return nullptr;
}
PCollider_ptr PPointHit::getCollider() const {
	physx::PxShape* sdkCollider = m_pPointHit->shape;
	std::weak_ptr<PCollider> wpCollider = *static_cast<std::weak_ptr<PCollider>*>(sdkCollider->userData);
	if (PCollider_ptr spActor = wpCollider.lock())
		return std::move(spActor);
	return nullptr;
}

glm::vec3 PPointHit::getPosition() const {
	if (!(m_pPointHit->flags & physx::PxHitFlag::ePOSITION)) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "position flag was not risen for this scene query", __FILE__, __LINE__);
		return glm::vec3(0.0f);
	}

	physx::PxVec3 position = m_pPointHit->position;
	return glm::vec3(position.x, position.y, position.z);
}
glm::vec3 PPointHit::getNormal() const {
	if (!(m_pPointHit->flags & physx::PxHitFlag::eNORMAL)) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "normal flag was not risen for this scene query", __FILE__, __LINE__);
		return glm::vec3(0.0f);
	}

	physx::PxVec3 normal = m_pPointHit->normal;
	return glm::vec3(normal.x, normal.y, normal.z);
}
float PPointHit::getDistance() const {
	return m_pPointHit->distance;
}
#pragma endregion

#pragma region Raycast Functions
void PRaycastHit::createHit(physx::PxRaycastHit* pRaycastHit) {
	m_pRaycastHit = new (PAlignedAllocator::allocate<physx::PxRaycastHit>()) physx::PxRaycastHit(*pRaycastHit);
	m_pPointHit = static_cast<physx::PxLocationHit*>(m_pRaycastHit);
}
#pragma endregion
#pragma region Sweep Functions
void PSweepHit::createHit(physx::PxSweepHit* pSweepHit) {
	m_pSweepHit = new (PAlignedAllocator::allocate<physx::PxSweepHit>()) physx::PxSweepHit(*pSweepHit);
	m_pPointHit = static_cast<physx::PxLocationHit*>(m_pSweepHit);
}
#pragma endregion
#pragma region Overlap Functions
void POverlapHit::createHit(physx::PxOverlapHit* pOverlapHit) {
	m_pOverlapHit = new (PAlignedAllocator::allocate<physx::PxOverlapHit>()) physx::PxOverlapHit(*pOverlapHit);
	m_pQueryHit = static_cast<physx::PxQueryHit*>(m_pOverlapHit);
}
#pragma endregion