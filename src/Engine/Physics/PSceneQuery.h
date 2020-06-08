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

class PRaycastHit;
class PSweepHit;
class POverlapHit;

typedef std::shared_ptr<PRaycastHit> PRaycastHit_ptr;
typedef std::shared_ptr<PSweepHit> PSweepHit_ptr;
typedef std::shared_ptr<POverlapHit> POverlapHit_ptr;

class PQueryHit {
public:
	~PQueryHit();

	PActor_ptr getActor() const;
	PCollider_ptr getCollider() const;

protected:
	physx::PxQueryHit* m_pQueryHit = nullptr;
};
class PPointHit {
public:
	~PPointHit();

	PActor_ptr getActor() const;
	PCollider_ptr getCollider() const;

	float getDistance() const;
	glm::vec3 getNormal() const;
	glm::vec3 getPosition() const;

protected:
	physx::PxLocationHit* m_pPointHit = nullptr;
};

class PRaycastHit : public PPointHit {
public:
	void createHit(physx::PxRaycastHit* pRaycastHit);

private:
	physx::PxRaycastHit* m_pRaycastHit = nullptr;
};
class PSweepHit : public PPointHit {
public:
	void createHit(physx::PxSweepHit* pSweepHit);

private:
	physx::PxSweepHit* m_pSweepHit = nullptr;
};
class POverlapHit : public PQueryHit {
public:
	void createHit(physx::PxOverlapHit* pOverlapHit);

private:
	physx::PxOverlapHit* m_pOverlapHit = nullptr;
};
