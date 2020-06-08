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
#include "PScene.h"
#include "PActor.h"
#include "PCollider.h"
#include "PGeometry.h"
#include "PMaterial.h"
#include "PJoint.h"

#define PVD_HOST "127.0.0.1"
#define PVD_PORT 5425

struct PhysicsSettings {
	/// Physics Object options
	bool bEnableHeightFields = false;
	bool bEnableArticulations = false;
	bool bEnableArticulationsReducedCoordinates = false;
	physx::PxTolerancesScale toleranceScale = physx::PxTolerancesScale();

	/// Miscellaneous
	//bool bEnableExtensions = false;
	bool bEnableVisualDebugger = false;
	bool bTrackMemoryAllocations = false;

	physx::PxAllocatorCallback* pAllocator = nullptr;
	physx::PxErrorCallback* pErrorReporter = nullptr;

	bool CreateFullPhysics() const;
};

class PhysicsEngine {
public:
	// Singleton used for PhysX initialization
	physx::PxFoundation* m_pFoundation = nullptr;
	// Singleton used for configuring the visual debugger
	physx::PxPvd* m_pVisualDebugger = nullptr;
	// Singleton used for instancing objects in the Physics SDK
	physx::PxPhysics* m_pPhysics = nullptr;
	// Provides utilities for creating, converting & serializing bulk data.
	physx::PxCooking* m_pCooking = nullptr;

	// Provides memory allocation/deallocation functionality
	physx::PxAllocatorCallback* m_pAllocator = nullptr;
	// Provides error logging functionality
	physx::PxErrorCallback* m_pErrorReporter = nullptr;


	bool init(const PhysicsSettings& options);
	void terminate();

private:
	bool initAllocator(const PhysicsSettings& settings);
	bool initErrorReporter(const PhysicsSettings& settings);
	bool initFoundation();
	bool initVisualDebugger();
	bool initPhysics(const PhysicsSettings& options);
	bool initCooking(const PhysicsSettings& options);
	bool initExtensions();
};

extern PhysicsEngine* g_pPhysicsEngine;