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
#include "PCollider.h"

class PActor;
class PStaticActor;
class PDynamicActor;

typedef std::shared_ptr<PActor> PActor_ptr;
typedef std::shared_ptr<PStaticActor> PStaticActor_ptr;
typedef std::shared_ptr<PDynamicActor> PDynamicActor_ptr;

class PActor {
	friend class PCollider;

public:
	~PActor();

	physx::PxRigidActor* getSdkActor() const;
	
	void addCollider(PCollider_ptr collider);
	void removeCollider(PCollider_ptr pCollider);


protected:
	// For non-specific physics operations
	physx::PxRigidActor* m_pActor = nullptr;

	std::list<PCollider_ptr> m_pColliders;
};
class PStaticActor : public PActor {
public:
	PStaticActor(physx::PxRigidStatic* pStaticActor);

	static PStaticActor_ptr createActor(glm::vec3 position = glm::vec3(0.0f));
	
private:
	// For specific physics operations
	physx::PxRigidStatic* m_pStaticActor = nullptr;
};
class PDynamicActor : public PActor {
public:
	PDynamicActor(physx::PxRigidDynamic* pDynamicActor);

	static PDynamicActor_ptr createActor(glm::vec3 position = glm::vec3(0.0f));

	void applyForce(glm::vec3 force);
	void applyImpulse(glm::vec3 impulse);
	void applyTorque(glm::vec3 torque);
	
	void UpdateMassAndInertia(float density);

	#pragma region Getter Functions
	glm::vec3 getPosition() const;
	float getMass() const;
	glm::vec3  getInertiaTensor() const;
	glm::vec3  getLinearVelocity() const;
	float getLinearDamping() const;
	glm::vec3  getAngularVelocity() const;
	float getAngularDamping() const;
	#pragma endregion

	#pragma region Setter Functions
	void setPosition(glm::vec3 position);
	void setMass(float mass);
	void setInertiaTensor(glm::vec3 interiaTensor);
	void setLinearVelocity(glm::vec3 velocity);
	void setLinearDamping(float damping);
	void setAngularVelocity(glm::vec3 velocity);
	void setAngularDamping(float damping);
	#pragma endregion

	#pragma region Utility Functions
	void setGravity(bool value);
	void setKinematic(bool value);

	enum class MotionAxis { 
		LINEAR_X  = (1 << 0), 
		LINEAR_Y  = (1 << 1), 
		LINEAR_Z  = (1 << 2), 
		ANGULAR_X = (1 << 3), 
		ANGULAR_Y = (1 << 4), 
		ANGULAR_Z = (1 << 5)
	};
	void lockMotion(MotionAxis axis);
	void unlockMotion(MotionAxis axis);
	#pragma endregion

private:
	// For specific physics operations
	physx::PxRigidDynamic* m_pDynamicActor = nullptr;
};