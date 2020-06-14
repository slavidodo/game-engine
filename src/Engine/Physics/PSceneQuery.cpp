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

PQueryHit::~PQueryHit() {
	PAlignedAllocator::deallocate(mQueryHit);
}

PActor_ptr PQueryHit::GetActor() const {
	physx::PxRigidActor* sdkActor = mQueryHit->actor;
	std::weak_ptr<PActor> wpActor = *static_cast<std::weak_ptr<PActor>*>(sdkActor->userData);
	if (PActor_ptr spActor = wpActor.lock())
		return std::move(spActor);
	return nullptr;
}
PCollider_ptr PQueryHit::GetCollider() const {
	physx::PxShape* sdkCollider = mQueryHit->shape;
	std::weak_ptr<PCollider> wpCollider = *static_cast<std::weak_ptr<PCollider>*>(sdkCollider->userData);
	if (PCollider_ptr spActor = wpCollider.lock())
		return std::move(spActor);
	return nullptr;
}

PPointHit::~PPointHit() {
	PAlignedAllocator::deallocate(mPointHit);
}

PActor_ptr PPointHit::GetActor() const {
	physx::PxRigidActor* sdkActor = mPointHit->actor;
	std::weak_ptr<PActor> wpActor = *static_cast<std::weak_ptr<PActor>*>(sdkActor->userData);
	
	if (PActor_ptr spActor = wpActor.lock())
		return std::move(spActor);
	return nullptr;
}
PCollider_ptr PPointHit::GetCollider() const {
	physx::PxShape* sdkCollider = mPointHit->shape;
	std::weak_ptr<PCollider> wpCollider = *static_cast<std::weak_ptr<PCollider>*>(sdkCollider->userData);
	if (PCollider_ptr spActor = wpCollider.lock())
		return std::move(spActor);
	return nullptr;
}

glm::vec3 PPointHit::GetPosition() const {
	if (!(mPointHit->flags & physx::PxHitFlag::ePOSITION)) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "position flag was not risen for this scene query", __FILE__, __LINE__);
		return glm::vec3(0.0f);
	}

	physx::PxVec3 position = mPointHit->position;
	return glm::vec3(position.x, position.y, position.z);
}
glm::vec3 PPointHit::GetNormal() const {
	if (!(mPointHit->flags & physx::PxHitFlag::eNORMAL)) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "normal flag was not risen for this scene query", __FILE__, __LINE__);
		return glm::vec3(0.0f);
	}

	physx::PxVec3 normal = mPointHit->normal;
	return glm::vec3(normal.x, normal.y, normal.z);
}
float PPointHit::GetDistance() const {
	return mPointHit->distance;
}

void PRaycastHit::CreateHit(physx::PxRaycastHit* pRaycastHit) {
	mRaycastHit = new (PAlignedAllocator::allocate<physx::PxRaycastHit>()) physx::PxRaycastHit(*pRaycastHit);
	mPointHit = static_cast<physx::PxLocationHit*>(mRaycastHit);
}

void PSweepHit::CreateHit(physx::PxSweepHit* pSweepHit) {
	mSweepHit = new (PAlignedAllocator::allocate<physx::PxSweepHit>()) physx::PxSweepHit(*pSweepHit);
	mPointHit = static_cast<physx::PxLocationHit*>(mSweepHit);
}

void POverlapHit::CreateHit(physx::PxOverlapHit* pOverlapHit) {
	mOverlapHit = new (PAlignedAllocator::allocate<physx::PxOverlapHit>()) physx::PxOverlapHit(*pOverlapHit);
	mQueryHit = static_cast<physx::PxQueryHit*>(mOverlapHit);
}
