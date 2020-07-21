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

PJoint::~PJoint() {
	if (!mJoint) return;

	PAlignedAllocator::deallocate(mJoint->userData);
	mJoint->release();
}

bool PJoint::IsBroken() {
	return mJoint->getConstraintFlags() & physx::PxConstraintFlag::eBROKEN;
}
void PJoint::SetThresholdForceAndTorque(float forceMagnitude, float torqueMagnitude) {
	mJoint->setBreakForce(forceMagnitude, torqueMagnitude);
}
void PJoint::SetVisualization(bool value) {
	mJoint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, value);
}

PFixedJoint::PFixedJoint(physx::PxFixedJoint* pFixedJoint) : mFixedJoint(pFixedJoint) {
	mJoint = static_cast<physx::PxJoint*>(mFixedJoint);
}
PFixedJoint_ptr PFixedJoint::CreateJoint(PhysicsActor_ptr actor1, PhysicsActor_ptr actor2) {
	physx::PxFixedJoint* pFixedJoint = physx::PxFixedJointCreate(*PhysicsEngine::GetInstance().mPhysics, actor1->GetSdkActor(), actor1->GetSdkActor()->getGlobalPose(), actor2->GetSdkActor(), actor2->GetSdkActor()->getGlobalPose());
	if (!pFixedJoint) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating fixed joint", __FILE__, __LINE__);
		return nullptr;
	}

	PFixedJoint_ptr joint = std::make_shared<PFixedJoint>(pFixedJoint);
	joint->mFixedJoint->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PFixedJoint>>()) std::weak_ptr<PFixedJoint>(joint);

	return std::move(joint);
}

PDistanceJoint::PDistanceJoint(physx::PxDistanceJoint* pDistanceJoint) : mDistanceJoint(pDistanceJoint) {
	mJoint = static_cast<physx::PxJoint*>(mDistanceJoint);
}
PDistanceJoint_ptr PDistanceJoint::CreateJoint(PhysicsActor_ptr actor1, PhysicsActor_ptr actor2) {
	physx::PxDistanceJoint* pDistanceJoint = physx::PxDistanceJointCreate(*PhysicsEngine::GetInstance().mPhysics, actor1->GetSdkActor(), actor1->GetSdkActor()->getGlobalPose(), actor2->GetSdkActor(), actor2->GetSdkActor()->getGlobalPose());
	if (!pDistanceJoint) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating Distance joint", __FILE__, __LINE__);
		return nullptr;
	}

	PDistanceJoint_ptr joint = std::make_shared<PDistanceJoint>(pDistanceJoint);
	joint->mDistanceJoint->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PDistanceJoint>>()) std::weak_ptr<PDistanceJoint>(joint);

	return std::move(joint);
}

void PDistanceJoint::SetLimits(float minDistance, float maxDistance, float tolerance) {
	mDistanceJoint->setMinDistance(minDistance);
	mDistanceJoint->setDistanceJointFlag(physx::PxDistanceJointFlag::eMIN_DISTANCE_ENABLED, true);
	
	mDistanceJoint->setMaxDistance(maxDistance);
	mDistanceJoint->setDistanceJointFlag(physx::PxDistanceJointFlag::eMAX_DISTANCE_ENABLED, true);

	mDistanceJoint->setTolerance(tolerance);
}

PSphericalJoint::PSphericalJoint(physx::PxSphericalJoint* pSphericalJoint) : mSphericalJoint(pSphericalJoint) {
	mJoint = static_cast<physx::PxJoint*>(mSphericalJoint);
}
PSphericalJoint_ptr PSphericalJoint::CreateJoint(PhysicsActor_ptr actor1, PhysicsActor_ptr actor2) {
	physx::PxSphericalJoint* pSphericalJoint = physx::PxSphericalJointCreate(*PhysicsEngine::GetInstance().mPhysics, actor1->GetSdkActor(), actor1->GetSdkActor()->getGlobalPose(), actor2->GetSdkActor(), actor2->GetSdkActor()->getGlobalPose());
	if (!pSphericalJoint) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating Spherical joint", __FILE__, __LINE__);
		return nullptr;
	}

	PSphericalJoint_ptr joint = std::make_shared<PSphericalJoint>(pSphericalJoint);
	joint->mSphericalJoint->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PSphericalJoint>>()) std::weak_ptr<PSphericalJoint>(joint);

	return std::move(joint);
}

void PSphericalJoint::SetLimits(float yMaxAngleRadians, float zMaxAngleRadians, float tolerance) {
	mSphericalJoint->setLimitCone(physx::PxJointLimitCone(yMaxAngleRadians, zMaxAngleRadians, tolerance));
	mSphericalJoint->setSphericalJointFlag(physx::PxSphericalJointFlag::eLIMIT_ENABLED, true);
}

PRevoluteJoint::PRevoluteJoint(physx::PxRevoluteJoint* pRevoluteJoint) : mRevoluteJoint(pRevoluteJoint) {
	mJoint = static_cast<physx::PxJoint*>(mRevoluteJoint);
}
PRevoluteJoint_ptr PRevoluteJoint::CreateJoint(PhysicsActor_ptr actor1, PhysicsActor_ptr actor2) {
	physx::PxRevoluteJoint* pRevoluteJoint = physx::PxRevoluteJointCreate(*PhysicsEngine::GetInstance().mPhysics, actor1->GetSdkActor(), actor1->GetSdkActor()->getGlobalPose(), actor2->GetSdkActor(), actor2->GetSdkActor()->getGlobalPose());
	if (!pRevoluteJoint) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating Revolute joint", __FILE__, __LINE__);
		return nullptr;
	}

	PRevoluteJoint_ptr joint = std::make_shared<PRevoluteJoint>(pRevoluteJoint);
	joint->mRevoluteJoint->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PRevoluteJoint>>()) std::weak_ptr<PRevoluteJoint>(joint);

	return std::move(joint);
}

void PRevoluteJoint::SetLimits(float minAngleRadian, float maxAngleRadian, float tolerance) {
	mRevoluteJoint->setLimit(physx::PxJointAngularLimitPair(minAngleRadian, maxAngleRadian, tolerance));
	mRevoluteJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, true);
}

PPrismaticJoint::PPrismaticJoint(physx::PxPrismaticJoint* pPrismaticJoint) : mPrismaticJoint(pPrismaticJoint) {
	mJoint = static_cast<physx::PxJoint*>(mPrismaticJoint);
}
PPrismaticJoint_ptr PPrismaticJoint::CreateJoint(PhysicsActor_ptr actor1, PhysicsActor_ptr actor2) {
	physx::PxPrismaticJoint* pPrismaticJoint = physx::PxPrismaticJointCreate(*PhysicsEngine::GetInstance().mPhysics, actor1->GetSdkActor(), actor1->GetSdkActor()->getGlobalPose(), actor2->GetSdkActor(), actor2->GetSdkActor()->getGlobalPose());
	if (!pPrismaticJoint) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating Prismatic joint", __FILE__, __LINE__);
		return nullptr;
	}

	PPrismaticJoint_ptr joint = std::make_shared<PPrismaticJoint>(pPrismaticJoint);
	joint->mPrismaticJoint->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PPrismaticJoint>>()) std::weak_ptr<PPrismaticJoint>(joint);

	return std::move(joint);
}

void PPrismaticJoint::SetLimits(float minDistance, float maxDistance, float tolerance) {
	mPrismaticJoint->setLimit(physx::PxJointLinearLimitPair(PhysicsEngine::GetInstance().mPhysics->getTolerancesScale(), minDistance, maxDistance, tolerance));
	mPrismaticJoint->setPrismaticJointFlag(physx::PxPrismaticJointFlag::eLIMIT_ENABLED, true);
}
