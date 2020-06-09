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

#pragma once
#include "physx/PxPhysicsAPI.h"
#include "PAllocator.h"

class PMaterial;
class PMaterialSet;

typedef std::shared_ptr<PMaterial> PMaterial_ptr;
typedef std::shared_ptr<PMaterialSet> PMaterialSet_ptr;

struct PMaterialProperties {
public:
	float staticFriction, dynamicFriction, restitution;
	PMaterialProperties(float staticFriction = 0.5f, float dynamicFriction = 0.5f, float restitution = 0.5f);
};
class PMaterial {
public:	
	PMaterial(physx::PxMaterial* pMaterial);
	~PMaterial();

	static PMaterial_ptr CreateMaterial(const PMaterialProperties& properties);
	static PMaterial_ptr CreateMaterial(float staticFriction = 0.5f, float dynamicFriction = 0.5f, float restitution = 0.5f);

	physx::PxMaterial* GetSdkMaterial() const;

private:	
	physx::PxMaterial* mMaterial = nullptr;
};
class PMaterialSet {
public:
	PMaterialSet(physx::PxMaterial** pMaterialArray, uint16_t numOfMaterials);
	~PMaterialSet();

	static PMaterialSet_ptr CreateMaterialSet(const std::vector<PMaterialProperties>& materialsProperties);

	physx::PxMaterial** GetSdkMaterialsArray() const;
	uint16_t GetNumOfMaterials() const;

private:
	uint16_t mNumOfMaterials;
	physx::PxMaterial** mMaterialsArray = nullptr;
};

extern PMaterial_ptr gDefaultMaterial;
