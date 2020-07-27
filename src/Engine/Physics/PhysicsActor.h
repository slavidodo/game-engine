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

#ifndef ENGINE_PHYSICS_ACTOR_H
#define ENGINE_PHYSICS_ACTOR_H

#include "physx/PxPhysicsAPI.h"
#include "PCollider.h"
#include "../Core/Transform.h"

class PhysicsActor;
class PStaticActor;
class PDynamicActor;

typedef std::shared_ptr<PhysicsActor> PhysicsActor_ptr;
typedef std::shared_ptr<PStaticActor> PStaticActor_ptr;
typedef std::shared_ptr<PDynamicActor> PDynamicActor_ptr;

class PhysicsActor 
{
	friend class PCollider;

public:
	~PhysicsActor();

	physx::PxRigidActor* GetSdkActor() const;
	
	void AddCollider(PCollider_ptr collider);
	void RemoveCollider(PCollider_ptr pCollider);

	glm::vec3 GetPosition() const;
	glm::quat GetRotation() const;
	Transform_ptr GetTransform() const;

protected:
	// For non-specific physics operations
	physx::PxRigidActor* mActor = nullptr;

	std::list<PCollider_ptr> mColliders;
};

class PStaticActor : public PhysicsActor 
{
public:
	PStaticActor(physx::PxRigidStatic* pStaticActor);

	static PStaticActor_ptr CreateActor(Transform_ptr transform = nullptr);
	
private:
	// For specific physics operations
	physx::PxRigidStatic* mStaticActor = nullptr;
};

class PDynamicActor : public PhysicsActor 
{
public:
	PDynamicActor(physx::PxRigidDynamic* pDynamicActor);

	static PDynamicActor_ptr CreateActor(Transform_ptr transform = nullptr);

	void ApplyForce(glm::vec3 force);
	void ApplyImpulse(glm::vec3 impulse);
	void ApplyTorque(glm::vec3 torque);
	
	void UpdateMassAndInertia(float density);

	//glm::vec3 GetPosition() const;
	float GetMass() const;
	glm::vec3  GetInertiaTensor() const;
	glm::vec3  GetLinearVelocity() const;
	float GetLinearDamping() const;
	glm::vec3  GetAngularVelocity() const;
	float GetAngularDamping() const;

	void SetPosition(glm::vec3 position);
	void SetMass(float mass);
	void SetInertiaTensor(glm::vec3 interiaTensor);
	void SetLinearVelocity(glm::vec3 velocity);
	void SetLinearDamping(float damping);
	void SetAngularVelocity(glm::vec3 velocity);
	void SetAngularDamping(float damping);

	void SetGravity(bool value);
	void SetKinematic(bool value);
	void SetKinematicTarget(Transform_ptr transform);
	Transform_ptr GetKinematicTarget();

	enum class MotionAxis { 
		LINEAR_X  = (1 << 0), 
		LINEAR_Y  = (1 << 1), 
		LINEAR_Z  = (1 << 2), 
		ANGULAR_X = (1 << 3), 
		ANGULAR_Y = (1 << 4), 
		ANGULAR_Z = (1 << 5)
	};
	void LockMotion(MotionAxis axis);
	void UnlockMotion(MotionAxis axis);

private:
	// For specific physics operations
	physx::PxRigidDynamic* mDynamicActor = nullptr;
};

#endif