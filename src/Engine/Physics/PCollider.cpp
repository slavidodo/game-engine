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
#include"PhysicsEngine.h"

PCollider::PCollider(physx::PxShape* pShape) : m_pShape(pShape) {
	setCollisionCategory(0);
}
PCollider::~PCollider() {
	if (!m_pShape) return;
	
	PAlignedAllocator::deallocate(m_pShape->userData);
	m_pShape->release();
}

PCollider_ptr PCollider::createCollider(const PGeometry_ptr geometry, const PMaterial_ptr material) {
	if (geometry->getSdkGeometry()->getType() == physx::PxGeometryType::eHEIGHTFIELD) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "This constructor overload is not suitable for height fields", __FILE__, __LINE__);
		return nullptr;
	}

	physx::PxShape* pShape = g_pPhysicsEngine->m_pPhysics->createShape(*geometry->getSdkGeometry(), *material->getSdkMaterial());
	if (!pShape) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating phsyx shape", __FILE__, __LINE__);
		return nullptr;
	}
	
	PCollider_ptr collider = std::make_shared<PCollider>(pShape);
	collider->m_pShape->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PCollider>>()) std::weak_ptr<PCollider>(collider);

	return std::move(collider);
}
PCollider_ptr PCollider::createCollider(const PGeometry_ptr geometry, const PMaterialSet_ptr materialSet) {
	physx::PxShape* pShape = g_pPhysicsEngine->m_pPhysics->createShape(*geometry->getSdkGeometry(), materialSet->getSdkMaterialsArray(), materialSet->getNumOfMaterials());
	if (!pShape) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating phsyx shape", __FILE__, __LINE__);
		return nullptr;
	}

	PCollider_ptr collider = std::make_shared<PCollider>(pShape);
	collider->m_pShape->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PCollider>>()) std::weak_ptr<PCollider>(collider);

	return std::move(collider);
}

physx::PxShape* PCollider::getSdkShape() const {
	return m_pShape;
}

void PCollider::setLocalPosition(glm::vec3 position) {
	m_pShape->setLocalPose(physx::PxTransform(position.x, position.y, position.z));
}
glm::vec3 PCollider::getLocalPosition() const {
	physx::PxTransform transform = m_pShape->getLocalPose();
	return glm::vec3(transform.p.x, transform.p.y, transform.p.z);
}

void PCollider::rotate(float angleRadian, glm::vec3 axis) {
	physx::PxVec3 rotationAxis = physx::PxVec3(axis.x, axis.y, axis.z).getNormalized();
	m_pShape->setLocalPose(physx::PxTransform(physx::PxQuat(angleRadian, rotationAxis)));
}

void PCollider::setTrigger(bool value) {
	m_pShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, value);
}
void PCollider::setSimulation(bool value) {
	m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, value);
}
void PCollider::setVisualization(bool value) {
	m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, value);
}
void PCollider::setCollisionCategory(uint8_t categoryNum) {
	if (categoryNum > 126) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "trying to assign category number higher than max (126)", __FILE__, __LINE__);
		return;
	}

	uint32_t words[4] = {0};
	words[categoryNum / 32] = 1 << (categoryNum % 32);
	m_pShape->setSimulationFilterData(physx::PxFilterData(words[0], words[1], words[2], words[3]));		
}