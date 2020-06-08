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
#include "PActor.h"

class PJoint;
class PFixedJoint;
class PDistanceJoint;
class PRevoluteJoint;
class PSphericalJoint;
class PPrismaticJoint;

typedef std::shared_ptr<PJoint> PJoint_ptr;
typedef std::shared_ptr<PFixedJoint> PFixedJoint_ptr;
typedef std::shared_ptr<PDistanceJoint> PDistanceJoint_ptr;
typedef std::shared_ptr<PRevoluteJoint> PRevoluteJoint_ptr;
typedef std::shared_ptr<PSphericalJoint> PSphericalJoint_ptr;
typedef std::shared_ptr<PPrismaticJoint> PPrismaticJoint_ptr;

class PJoint {
public:
	~PJoint();

	bool isBroken();
	void setThresholdForceAndTorque(float forceMagnitude, float torqueMagnitude);
	void setVisualization(bool value); // Joint visualization doesn't work for some reason?

protected:
	physx::PxJoint* m_pJoint;
};

class PFixedJoint : public PJoint {
public:
	PFixedJoint(physx::PxFixedJoint* pFixedJoint);
	static PFixedJoint_ptr createJoint(PActor_ptr actor1, PActor_ptr actor2);

private:
	physx::PxFixedJoint* m_pFixedJoint;
};
class PDistanceJoint : public PJoint {
public:
	PDistanceJoint(physx::PxDistanceJoint* pDistanceJoint);
	static PDistanceJoint_ptr createJoint(PActor_ptr actor1, PActor_ptr actor2);

	void setLimits(float minDistance, float maxDistance, float tolerance = -1.0f);

private:
	physx::PxDistanceJoint* m_pDistanceJoint;
};
class PSphericalJoint : public PJoint {
public:
	PSphericalJoint(physx::PxSphericalJoint* pSphericalJoint);
	static PSphericalJoint_ptr createJoint(PActor_ptr actor1, PActor_ptr actor2);

	void setLimits(float yMaxAngleRadians, float zMaxAngleRadians, float tolerance = -1.0f);

private:
	physx::PxSphericalJoint* m_pSphericalJoint;
};
class PRevoluteJoint : public PJoint {
public:
	PRevoluteJoint(physx::PxRevoluteJoint* pRevoluteJoint);
	static PRevoluteJoint_ptr createJoint(PActor_ptr actor1, PActor_ptr actor2);

	void setLimits(float minAngleRadian, float maxAngleRadian, float tolerance = -1.0f);

private:
	physx::PxRevoluteJoint* m_pRevoluteJoint;
};
class PPrismaticJoint : public PJoint {
public:
	PPrismaticJoint(physx::PxPrismaticJoint* pPrismaticJoint);
	static PPrismaticJoint_ptr createJoint(PActor_ptr actor1, PActor_ptr actor2);

	void setLimits(float minDistance, float maxDistance, float tolerance = -1.0f);

private:
	physx::PxPrismaticJoint* m_pPrismaticJoint;
};