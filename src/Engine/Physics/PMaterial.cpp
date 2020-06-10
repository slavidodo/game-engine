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

PMaterial_ptr gDefaultMaterial = nullptr;

PMaterialProperties::PMaterialProperties(float staticFriction, float dynamicFriction, float restitution) : staticFriction(staticFriction), dynamicFriction(dynamicFriction), restitution(restitution) {}

PMaterial::PMaterial(physx::PxMaterial* pMaterial) : mMaterial(pMaterial) {}
PMaterial::~PMaterial() {
	if (!mMaterial) return;
	
	PAlignedAllocator::deallocate(mMaterial->userData);
	mMaterial->release();
}

PMaterial_ptr PMaterial::CreateMaterial(const PMaterialProperties& properties) {
	physx::PxMaterial* pMaterial = PhysicsEngine::GetInstance().mPhysics->createMaterial(properties.staticFriction, properties.dynamicFriction, properties.restitution);
	if (!pMaterial) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating material", __FILE__, __LINE__);
		return nullptr;
	}

	PMaterial_ptr material = std::make_shared<PMaterial>(pMaterial);
	material->mMaterial->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PMaterial>>()) std::weak_ptr<PMaterial>(material);

	return std::move(material);
}
PMaterial_ptr PMaterial::CreateMaterial(float staticFriction, float dynamicFriction, float restitution) {
	physx::PxMaterial* pMaterial = PhysicsEngine::GetInstance().mPhysics->createMaterial(staticFriction, dynamicFriction, restitution);
	if (!pMaterial) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Error creating material", __FILE__, __LINE__);
		return nullptr;
	}

	PMaterial_ptr material = std::make_shared<PMaterial>(pMaterial);
	material->mMaterial->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PMaterial>>()) std::weak_ptr<PMaterial>(material);

	return std::move(material);
}

physx::PxMaterial* PMaterial::GetSdkMaterial() const { 
	return mMaterial;
}

PMaterialSet::PMaterialSet(physx::PxMaterial** pMaterialArray, uint16_t numOfMaterials) : mMaterialsArray(pMaterialArray), mNumOfMaterials(numOfMaterials) {}
PMaterialSet::~PMaterialSet() {
	if (!mMaterialsArray) return;
	
	PAlignedAllocator::deallocate(mMaterialsArray[0]->userData);

	for (int i = 0; i < mNumOfMaterials; i++)
		mMaterialsArray[i]->release();
	
	PAlignedAllocator::deallocate(mMaterialsArray);
}

PMaterialSet_ptr PMaterialSet::CreateMaterialSet(const std::vector<PMaterialProperties>& materialsProperties) {
	uint16_t numOfMaterials = static_cast<uint16_t>(materialsProperties.size());
	physx::PxMaterial** pMaterialsArray = PAlignedAllocator::allocate<physx::PxMaterial*>(numOfMaterials);
	
	for (int i = 0; i < numOfMaterials; i++) {
		PMaterialProperties properties = materialsProperties[i];
		pMaterialsArray[i] = PhysicsEngine::GetInstance().mPhysics->createMaterial(properties.staticFriction, properties.dynamicFriction, properties.restitution);
	}

	PMaterialSet_ptr materialArray = std::make_shared<PMaterialSet>(pMaterialsArray, numOfMaterials);
	materialArray->mMaterialsArray[0]->userData = new (PAlignedAllocator::allocate<std::weak_ptr<PMaterialSet>>()) std::weak_ptr<PMaterialSet>(materialArray);
	
	return std::move(materialArray);
}

physx::PxMaterial** PMaterialSet::GetSdkMaterialsArray() const {
	return mMaterialsArray;
}

uint16_t PMaterialSet::GetNumOfMaterials() const {
	return mNumOfMaterials;
}
