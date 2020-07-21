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

#ifndef ENGINE_PHYSICS_COLLIDER_H
#define ENGINE_PHYSICS_COLLIDER_H

#include "physx/PxPhysicsAPI.h"
#include "PGeometry.h"
#include "PMaterial.h"

class PhysicsActor;
class PCollider;
typedef std::shared_ptr<PhysicsActor> PhysicsActor_ptr;
typedef std::shared_ptr<PCollider> PCollider_ptr;

class PCollider 
{
friend class PhysicsActor;

public:
	PCollider(physx::PxShape* shape);
	~PCollider();

	static PCollider_ptr CreateCollider(const PGeometry_ptr geometry, const PMaterial_ptr pMaterial = gDefaultMaterial);
	// This overload is mandatory for height fields
	static PCollider_ptr CreateCollider(const PGeometry_ptr geometry, const PMaterialSet_ptr materialSet);

	physx::PxShape* GetSdkShape() const;

	void SetLocalPosition(glm::vec3 position);
	glm::vec3 GetLocalPosition() const;
	
	void Rotate(float angleRadian, glm::vec3 axis);

	void SetTrigger(bool value);
	void SetSimulation(bool value);
	void SetVisualization(bool value);
	void SetCollisionCategory(uint8_t categoryNum);

private:
	physx::PxShape* mShape = nullptr;
};

#endif
