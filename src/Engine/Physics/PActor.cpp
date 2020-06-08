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

#pragma region Actor Functions
PActor::~PActor() {
	if (!m_pActor) return;
	
	PAlignedAllocator::deallocate(m_pActor->userData);
	m_pActor->release();
}

physx::PxRigidActor* PActor::getSdkActor() const {
	return m_pActor;
}

void PActor::addCollider(PCollider_ptr collider) {	
	m_pActor->attachShape(*collider->getSdkShape());
	m_pColliders.push_back(collider);
}
void PActor::removeCollider(PCollider_ptr pCollider) {
	auto it = std::find(m_pColliders.begin(), m_pColliders.end(), pCollider);
	if (it == m_pColliders.end()) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Trying to remove a collider that is not attached to this actor", __FILE__, __LINE__);
		return;
	}
	
	m_pActor->detachShape(*pCollider->m_pShape);
	m_pColliders.erase(it);
}
#pragma endregion

#pragma region Dynamic Actor Functions
PDynamicActor::PDynamicActor(physx::PxRigidDynamic* pDynamicActor) : m_pDynamicActor(pDynamicActor) {
	m_pActor = static_cast<physx::PxRigidActor*>(m_pDynamicActor);
}

PDynamicActor_ptr PDynamicActor::createActor(glm::vec3 position) {
	physx::PxRigidDynamic* pDynamicActor = g_pPhysicsEngine->m_pPhysics->createRigidDynamic(physx::PxTransform(position.x, position.y, position.z));
	if (!pDynamicActor) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating dynamic actor", __FILE__, __LINE__);
		return nullptr;
	}

	PDynamicActor_ptr actor = std::make_shared<PDynamicActor>(pDynamicActor);
	actor->m_pActor->userData = actor->m_pDynamicActor->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PDynamicActor>>()) std::weak_ptr<PDynamicActor>(actor);

	return std::move(actor);
}

void PDynamicActor::applyForce(glm::vec3 force) {
	m_pDynamicActor->addForce(physx::PxVec3(force.x, force.y, force.z));
}
void PDynamicActor::applyImpulse(glm::vec3 impulse) {
	m_pDynamicActor->addForce(physx::PxVec3(impulse.x, impulse.y, impulse.z), physx::PxForceMode::eIMPULSE);
}
void PDynamicActor::applyTorque(glm::vec3 torque) {
	m_pDynamicActor->addTorque(physx::PxVec3(torque.x, torque.y, torque.z));
}

void PDynamicActor::UpdateMassAndInertia(float density) {
	if (!physx::PxRigidBodyExt::updateMassAndInertia(*m_pDynamicActor, density)) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Failed to update mass & intertia of dynamic actor", __FILE__, __LINE__);
		return;
	}
}

#pragma region Getter Functions
glm::vec3 PDynamicActor::getPosition() const {
	physx::PxTransform transform = m_pDynamicActor->getGlobalPose();
	return glm::vec3(transform.p.x, transform.p.y, transform.p.z);
}

float PDynamicActor::getMass() const {
	return m_pDynamicActor->getMass();
}
glm::vec3 PDynamicActor::getInertiaTensor() const {
	physx::PxVec3 inertia = m_pDynamicActor->getMassSpaceInertiaTensor();
	return glm::vec3(inertia.x, inertia.y, inertia.z);
}

glm::vec3 PDynamicActor::getLinearVelocity() const {
	physx::PxVec3 velocity = m_pDynamicActor->getLinearVelocity();
	return glm::vec3(velocity.x, velocity.y, velocity.z);
}
float PDynamicActor::getLinearDamping() const {
	return m_pDynamicActor->getLinearDamping();
}

glm::vec3 PDynamicActor::getAngularVelocity() const {
	physx::PxVec3 velocity = m_pDynamicActor->getAngularVelocity();
	return glm::vec3(velocity.x, velocity.y, velocity.z);
}
float PDynamicActor::getAngularDamping() const {
	return m_pDynamicActor->getAngularDamping();
}
#pragma endregion

#pragma region Setter Functions
void PDynamicActor::setPosition(glm::vec3 position) {
	m_pDynamicActor->setGlobalPose(physx::PxTransform(position.x, position.y, position.z));
}

void PDynamicActor::setMass(float mass) {
	m_pDynamicActor->setMass(mass);
}
void PDynamicActor::setInertiaTensor(glm::vec3 inertia) {
	m_pDynamicActor->setMassSpaceInertiaTensor(physx::PxVec3(inertia.x, inertia.y, inertia.z));
}

void PDynamicActor::setLinearVelocity(glm::vec3 velocity) {
	m_pDynamicActor->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
}
void PDynamicActor::setLinearDamping(float damping) {
	m_pDynamicActor->setLinearDamping(damping);
}

void PDynamicActor::setAngularVelocity(glm::vec3 velocity) {
	m_pDynamicActor->setAngularVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
}
void PDynamicActor::setAngularDamping(float damping) {
	m_pDynamicActor->setAngularDamping(damping);
}
#pragma endregion

#pragma region Utility Functions
void PDynamicActor::setGravity(bool value) {
	m_pDynamicActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !value);
}
void PDynamicActor::setKinematic(bool value) {
	m_pDynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, value);
}

void PDynamicActor::lockMotion(MotionAxis axis) {
	m_pDynamicActor->setRigidDynamicLockFlag(static_cast<physx::PxRigidDynamicLockFlag::Enum>(axis), true);
}
void PDynamicActor::unlockMotion(MotionAxis axis) {
	m_pDynamicActor->setRigidDynamicLockFlag(static_cast<physx::PxRigidDynamicLockFlag::Enum>(axis), false);
}
#pragma endregion
#pragma endregion

#pragma region Static Actor Functions
PStaticActor::PStaticActor(physx::PxRigidStatic* pStaticActor) : m_pStaticActor(pStaticActor) {
	m_pActor = static_cast<physx::PxRigidActor*>(m_pStaticActor);
}

PStaticActor_ptr PStaticActor::createActor(glm::vec3 position) {
	physx::PxRigidStatic* pStaticActor = g_pPhysicsEngine->m_pPhysics->createRigidStatic(physx::PxTransform(position.x, position.y, position.z));
	if (!pStaticActor) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating static actor", __FILE__, __LINE__);
		return nullptr;
	}

	PStaticActor_ptr actor = std::make_shared<PStaticActor>(pStaticActor);
	actor->m_pActor->userData = actor->m_pStaticActor->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PStaticActor>>()) std::weak_ptr<PStaticActor>(actor);

	return std::move(actor);
}
#pragma endregion
