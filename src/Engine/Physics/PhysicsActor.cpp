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

PhysicsActor::~PhysicsActor() {
	if (!mActor) return;
	
	PAlignedAllocator::deallocate(mActor->userData);
	mActor->release();
}

physx::PxRigidActor* PhysicsActor::GetSdkActor() const {
	return mActor;
}

void PhysicsActor::AddCollider(PCollider_ptr collider) {	
	mActor->attachShape(*collider->GetSdkShape());
	mColliders.push_back(collider);
}
void PhysicsActor::RemoveCollider(PCollider_ptr pCollider) {
	auto it = std::find(mColliders.begin(), mColliders.end(), pCollider);
	if (it == mColliders.end()) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Trying to remove a collider that is not attached to this actor", __FILE__, __LINE__);
		return;
	}
	
	mActor->detachShape(*pCollider->mShape);
	mColliders.erase(it);
}

glm::fvec3 PhysicsActor::GetPosition() const {
	physx::PxTransform transform = mActor->getGlobalPose();
	return glm::fvec3(transform.p.x, transform.p.y, transform.p.z);
}
glm::fquat PhysicsActor::GetRotation() const {
	physx::PxTransform transform = mActor->getGlobalPose();
	return glm::fquat(transform.q.x, transform.q.y, transform.q.z, transform.q.w);
}
Transform_ptr PhysicsActor::GetTransform() const {
	physx::PxTransform transform = mActor->getGlobalPose();
	return std::move(PhysicsEngine::GetInstance().ToEngineTransform(transform));
}


PStaticActor::PStaticActor(physx::PxRigidStatic* pStaticActor) : mStaticActor(pStaticActor) {
	mActor = static_cast<physx::PxRigidActor*>(mStaticActor);
}

PStaticActor_ptr PStaticActor::CreateActor(Transform_ptr transform) {
	physx::PxRigidStatic* pStaticActor = PhysicsEngine::GetInstance().mPhysics->createRigidStatic(PhysicsEngine::GetInstance().ToPhysxTransform(transform));
	if (!pStaticActor) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating static actor", __FILE__, __LINE__);
		return nullptr;
	}

	PStaticActor_ptr actor = std::make_shared<PStaticActor>(pStaticActor);
	actor->mActor->userData = actor->mStaticActor->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PStaticActor>>()) std::weak_ptr<PStaticActor>(actor);

	return std::move(actor);
}




PDynamicActor::PDynamicActor(physx::PxRigidDynamic* pDynamicActor) : mDynamicActor(pDynamicActor) {
	mActor = static_cast<physx::PxRigidActor*>(mDynamicActor);
}

PDynamicActor_ptr PDynamicActor::CreateActor(Transform_ptr transform) {
	if (!transform) transform = std::make_shared<Transform>();
	physx::PxRigidDynamic* pDynamicActor = PhysicsEngine::GetInstance().mPhysics->createRigidDynamic(PhysicsEngine::GetInstance().ToPhysxTransform(transform));
	if (!pDynamicActor) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating dynamic actor", __FILE__, __LINE__);
		return nullptr;
	}

	PDynamicActor_ptr actor = std::make_shared<PDynamicActor>(pDynamicActor);
	actor->mActor->userData = actor->mDynamicActor->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PDynamicActor>>()) std::weak_ptr<PDynamicActor>(actor);

	return std::move(actor);
}

void PDynamicActor::ApplyForce(glm::vec3 force) {
	mDynamicActor->addForce(physx::PxVec3(force.x, force.y, force.z));
}
void PDynamicActor::ApplyImpulse(glm::vec3 impulse) {
	mDynamicActor->addForce(physx::PxVec3(impulse.x, impulse.y, impulse.z), physx::PxForceMode::eIMPULSE);
}
void PDynamicActor::ApplyTorque(glm::vec3 torque) {
	mDynamicActor->addTorque(physx::PxVec3(torque.x, torque.y, torque.z));
}

void PDynamicActor::UpdateMassAndInertia(float density) {
	if (!physx::PxRigidBodyExt::updateMassAndInertia(*mDynamicActor, density)) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Failed to Update mass & intertia of dynamic actor", __FILE__, __LINE__);
		return;
	}
}

float PDynamicActor::GetMass() const {
	return mDynamicActor->getMass();
}
glm::vec3 PDynamicActor::GetInertiaTensor() const {
	physx::PxVec3 inertia = mDynamicActor->getMassSpaceInertiaTensor();
	return glm::vec3(inertia.x, inertia.y, inertia.z);
}

glm::vec3 PDynamicActor::GetLinearVelocity() const {
	physx::PxVec3 velocity = mDynamicActor->getLinearVelocity();
	return glm::vec3(velocity.x, velocity.y, velocity.z);
}
float PDynamicActor::GetLinearDamping() const {
	return mDynamicActor->getLinearDamping();
}

glm::vec3 PDynamicActor::GetAngularVelocity() const {
	physx::PxVec3 velocity = mDynamicActor->getAngularVelocity();
	return glm::vec3(velocity.x, velocity.y, velocity.z);
}
float PDynamicActor::GetAngularDamping() const {
	return mDynamicActor->getAngularDamping();
}

void PDynamicActor::SetPosition(glm::vec3 position) {
	mDynamicActor->setGlobalPose(physx::PxTransform(position.x, position.y, position.z));
}

void PDynamicActor::SetMass(float mass) {
	mDynamicActor->setMass(mass);
}
void PDynamicActor::SetInertiaTensor(glm::vec3 inertia) {
	mDynamicActor->setMassSpaceInertiaTensor(physx::PxVec3(inertia.x, inertia.y, inertia.z));
}

void PDynamicActor::SetLinearVelocity(glm::vec3 velocity) {
	mDynamicActor->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
}
void PDynamicActor::SetLinearDamping(float damping) {
	mDynamicActor->setLinearDamping(damping);
}

void PDynamicActor::SetAngularVelocity(glm::vec3 velocity) {
	mDynamicActor->setAngularVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
}
void PDynamicActor::SetAngularDamping(float damping) {
	mDynamicActor->setAngularDamping(damping);
}

void PDynamicActor::SetGravity(bool value) {
	mDynamicActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !value);
}
void PDynamicActor::SetKinematic(bool value) {
	mDynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, value);
}
void PDynamicActor::SetKinematicTarget(Transform_ptr transform) {
	const physx::PxTransform newTransform = PhysicsEngine::GetInstance().ToPhysxTransform(transform);
	mDynamicActor->setKinematicTarget(newTransform);
}

Transform_ptr PDynamicActor::GetKinematicTarget() {
	physx::PxTransform transform;
	mDynamicActor->getKinematicTarget(transform);
	return PhysicsEngine::GetInstance().ToEngineTransform(transform);
}

void PDynamicActor::LockMotion(MotionAxis axis) {
	mDynamicActor->setRigidDynamicLockFlag(static_cast<physx::PxRigidDynamicLockFlag::Enum>(axis), true);
}
void PDynamicActor::UnlockMotion(MotionAxis axis) {
	mDynamicActor->setRigidDynamicLockFlag(static_cast<physx::PxRigidDynamicLockFlag::Enum>(axis), false);
}
