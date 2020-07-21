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

#ifndef ENGINE_PHYSICS_ENGINE_H
#define ENGINE_PHYSICS_ENGINE_H

#include "physx/PxPhysicsAPI.h"
#include "PAllocator.h"
#include "../Scene/PhysicsScene.h"
#include "PhysicsActor.h"
#include "PCollider.h"
#include "PGeometry.h"
#include "PMaterial.h"
#include "PJoint.h"
#include "../Core/Transform.h"

#define PVD_HOST "127.0.0.1"
#define PVD_PORT 5425

struct PhysicsSettings 
{
	/// Physics Object options
	bool bEnableHeightFields = false;
	bool bEnableArticulations = false;
	bool bEnableArticulationsReducedCoordinates = false;
	physx::PxTolerancesScale toleranceScale = physx::PxTolerancesScale();

	/// Miscellaneous
	bool bEnableVisualDebugger = false;
	bool bTrackMemoryAllocations = false;

	physx::PxAllocatorCallback* pAllocator = nullptr;
	physx::PxErrorCallback* pErrorReporter = nullptr;

	bool CreateFullPhysics() const;
};

class PhysicsEngine 
{
public:
	static PhysicsEngine& GetInstance() {
		static PhysicsEngine instance;
		return instance;
	}

	// Singleton used for PhysX initialization
	physx::PxFoundation* mFoundation = nullptr;
	// Singleton used for configuring the visual debugger
	physx::PxPvd* mVisualDebugger = nullptr;
	// Singleton used for instancing objects in the Physics SDK
	physx::PxPhysics* mPhysics = nullptr;
	// Provides utilities for creating, converting & serializing bulk data.
	physx::PxCooking* mCooking = nullptr;

	// Provides memory allocation/deallocation functionality
	physx::PxAllocatorCallback* mAllocator = nullptr;
	// Provides error logging functionality
	physx::PxErrorCallback* mErrorReporter = nullptr;


	bool Init(const PhysicsSettings& options);
	void Terminate();

	static physx::PxTransform ToPhysxTransform(Transform_ptr transform);
	static Transform_ptr ToEngineTransform(physx::PxTransform transform);

private:
	bool mInitialized = false;

	bool InitAllocator(const PhysicsSettings& settings);
	bool InitErrorReporter(const PhysicsSettings& settings);
	bool InitFoundation();
	bool InitVisualDebugger();
	bool InitPhysics(const PhysicsSettings& options);
	bool InitCooking(const PhysicsSettings& options);
	bool InitExtensions();
};

#endif