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

#pragma region Joint Functions
PJoint::~PJoint() {
	if (!m_pJoint) return;

	PAlignedAllocator::deallocate(m_pJoint->userData);
	m_pJoint->release();
}

bool PJoint::isBroken() {
	return m_pJoint->getConstraintFlags() & physx::PxConstraintFlag::eBROKEN;
}
void PJoint::setThresholdForceAndTorque(float forceMagnitude, float torqueMagnitude) {
	m_pJoint->setBreakForce(forceMagnitude, torqueMagnitude);
}
void PJoint::setVisualization(bool value) {
	m_pJoint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, value);
}
#pragma endregion

#pragma region Fixed Joint Functions
PFixedJoint::PFixedJoint(physx::PxFixedJoint* pFixedJoint) : m_pFixedJoint(pFixedJoint) {
	m_pJoint = static_cast<physx::PxJoint*>(m_pFixedJoint);
}
PFixedJoint_ptr PFixedJoint::createJoint(PActor_ptr actor1, PActor_ptr actor2) {
	physx::PxFixedJoint* pFixedJoint = physx::PxFixedJointCreate(*g_pPhysicsEngine->m_pPhysics, actor1->getSdkActor(), actor1->getSdkActor()->getGlobalPose(), actor2->getSdkActor(), actor2->getSdkActor()->getGlobalPose());
	if (!pFixedJoint) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating fixed joint", __FILE__, __LINE__);
		return nullptr;
	}

	PFixedJoint_ptr joint = std::make_shared<PFixedJoint>(pFixedJoint);
	joint->m_pFixedJoint->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PFixedJoint>>()) std::weak_ptr<PFixedJoint>(joint);

	return std::move(joint);
}
#pragma endregion
#pragma region Distance Joint Functions
PDistanceJoint::PDistanceJoint(physx::PxDistanceJoint* pDistanceJoint) : m_pDistanceJoint(pDistanceJoint) {
	m_pJoint = static_cast<physx::PxJoint*>(m_pDistanceJoint);
}
PDistanceJoint_ptr PDistanceJoint::createJoint(PActor_ptr actor1, PActor_ptr actor2) {
	physx::PxDistanceJoint* pDistanceJoint = physx::PxDistanceJointCreate(*g_pPhysicsEngine->m_pPhysics, actor1->getSdkActor(), actor1->getSdkActor()->getGlobalPose(), actor2->getSdkActor(), actor2->getSdkActor()->getGlobalPose());
	if (!pDistanceJoint) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating Distance joint", __FILE__, __LINE__);
		return nullptr;
	}

	PDistanceJoint_ptr joint = std::make_shared<PDistanceJoint>(pDistanceJoint);
	joint->m_pDistanceJoint->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PDistanceJoint>>()) std::weak_ptr<PDistanceJoint>(joint);

	return std::move(joint);
}

void PDistanceJoint::setLimits(float minDistance, float maxDistance, float tolerance) {
	m_pDistanceJoint->setMinDistance(minDistance);
	m_pDistanceJoint->setDistanceJointFlag(physx::PxDistanceJointFlag::eMIN_DISTANCE_ENABLED, true);
	
	m_pDistanceJoint->setMaxDistance(maxDistance);
	m_pDistanceJoint->setDistanceJointFlag(physx::PxDistanceJointFlag::eMAX_DISTANCE_ENABLED, true);

	m_pDistanceJoint->setTolerance(tolerance);
}
#pragma endregion
#pragma region Spherical Joint Functions
PSphericalJoint::PSphericalJoint(physx::PxSphericalJoint* pSphericalJoint) : m_pSphericalJoint(pSphericalJoint) {
	m_pJoint = static_cast<physx::PxJoint*>(m_pSphericalJoint);
}
PSphericalJoint_ptr PSphericalJoint::createJoint(PActor_ptr actor1, PActor_ptr actor2) {
	physx::PxSphericalJoint* pSphericalJoint = physx::PxSphericalJointCreate(*g_pPhysicsEngine->m_pPhysics, actor1->getSdkActor(), actor1->getSdkActor()->getGlobalPose(), actor2->getSdkActor(), actor2->getSdkActor()->getGlobalPose());
	if (!pSphericalJoint) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating Spherical joint", __FILE__, __LINE__);
		return nullptr;
	}

	PSphericalJoint_ptr joint = std::make_shared<PSphericalJoint>(pSphericalJoint);
	joint->m_pSphericalJoint->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PSphericalJoint>>()) std::weak_ptr<PSphericalJoint>(joint);

	return std::move(joint);
}

void PSphericalJoint::setLimits(float yMaxAngleRadians, float zMaxAngleRadians, float tolerance) {
	m_pSphericalJoint->setLimitCone(physx::PxJointLimitCone(yMaxAngleRadians, zMaxAngleRadians, tolerance));
	m_pSphericalJoint->setSphericalJointFlag(physx::PxSphericalJointFlag::eLIMIT_ENABLED, true);
}
#pragma endregion
#pragma region Revolute Joint Functions
PRevoluteJoint::PRevoluteJoint(physx::PxRevoluteJoint* pRevoluteJoint) : m_pRevoluteJoint(pRevoluteJoint) {
	m_pJoint = static_cast<physx::PxJoint*>(m_pRevoluteJoint);
}
PRevoluteJoint_ptr PRevoluteJoint::createJoint(PActor_ptr actor1, PActor_ptr actor2) {
	physx::PxRevoluteJoint* pRevoluteJoint = physx::PxRevoluteJointCreate(*g_pPhysicsEngine->m_pPhysics, actor1->getSdkActor(), actor1->getSdkActor()->getGlobalPose(), actor2->getSdkActor(), actor2->getSdkActor()->getGlobalPose());
	if (!pRevoluteJoint) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating Revolute joint", __FILE__, __LINE__);
		return nullptr;
	}

	PRevoluteJoint_ptr joint = std::make_shared<PRevoluteJoint>(pRevoluteJoint);
	joint->m_pRevoluteJoint->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PRevoluteJoint>>()) std::weak_ptr<PRevoluteJoint>(joint);

	return std::move(joint);
}

void PRevoluteJoint::setLimits(float minAngleRadian, float maxAngleRadian, float tolerance) {
	m_pRevoluteJoint->setLimit(physx::PxJointAngularLimitPair(minAngleRadian, maxAngleRadian, tolerance));
	m_pRevoluteJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, true);
}
#pragma endregion
#pragma region Prismatic Joint Functions
PPrismaticJoint::PPrismaticJoint(physx::PxPrismaticJoint* pPrismaticJoint) : m_pPrismaticJoint(pPrismaticJoint) {
	m_pJoint = static_cast<physx::PxJoint*>(m_pPrismaticJoint);
}
PPrismaticJoint_ptr PPrismaticJoint::createJoint(PActor_ptr actor1, PActor_ptr actor2) {
	physx::PxPrismaticJoint* pPrismaticJoint = physx::PxPrismaticJointCreate(*g_pPhysicsEngine->m_pPhysics, actor1->getSdkActor(), actor1->getSdkActor()->getGlobalPose(), actor2->getSdkActor(), actor2->getSdkActor()->getGlobalPose());
	if (!pPrismaticJoint) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating Prismatic joint", __FILE__, __LINE__);
		return nullptr;
	}

	PPrismaticJoint_ptr joint = std::make_shared<PPrismaticJoint>(pPrismaticJoint);
	joint->m_pPrismaticJoint->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PPrismaticJoint>>()) std::weak_ptr<PPrismaticJoint>(joint);

	return std::move(joint);
}

void PPrismaticJoint::setLimits(float minDistance, float maxDistance, float tolerance) {
	m_pPrismaticJoint->setLimit(physx::PxJointLinearLimitPair(g_pPhysicsEngine->m_pPhysics->getTolerancesScale(), minDistance, maxDistance, tolerance));
	m_pPrismaticJoint->setPrismaticJointFlag(physx::PxPrismaticJointFlag::eLIMIT_ENABLED, true);
}
#pragma endregion
