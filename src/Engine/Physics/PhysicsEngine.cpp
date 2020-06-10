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

#pragma region Initializtion/Termination
bool PhysicsEngine::Init(const PhysicsSettings& settings) {
	if (mInitialized)
		return true;

	InitAllocator(settings);
	InitErrorReporter(settings);

	if (!InitFoundation())
		return false;
	#ifdef _DEBUG
	if (settings.bEnableVisualDebugger && !InitVisualDebugger())
		return false;
	#endif
	if (!InitPhysics(settings))
		return false;
	if (!InitCooking(settings))
		return false;
	if (!InitExtensions())
		return false;

	mInitialized = true;
	gDefaultMaterial = PMaterial::CreateMaterial(0.5f, 0.5f, 0.5f);

	return true;
}
void PhysicsEngine::Terminate() {
	PSceneManager::GetInstance().SetCurrentScene(nullptr);

	gDefaultMaterial.reset();

	PxCloseExtensions();
	
	if (mCooking)
		mCooking->release();
	if (mPhysics)
		mPhysics->release();
	#ifdef _DEBUG
	if (mVisualDebugger)
		mVisualDebugger->release();
	#endif
	if (mFoundation)
		mFoundation->release();
	
	PAlignedAllocator::deallocate(mErrorReporter);
	PAlignedAllocator::deallocate(mAllocator);

	mInitialized = false;
}

bool PhysicsEngine::InitAllocator(const PhysicsSettings& settings) {
	mAllocator = new (PAlignedAllocator::allocate<SdkAllocator>()) SdkAllocator();
	if (!mAllocator) {
		mErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating allocator component", __FILE__, __LINE__);
		return false;
	}
	return true;
}
bool PhysicsEngine::InitErrorReporter(const PhysicsSettings& settings) {
	mErrorReporter = new (PAlignedAllocator::allocate<physx::PxDefaultErrorCallback>()) physx::PxDefaultErrorCallback();
	if (!mErrorReporter) {
		mErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating error callback component", __FILE__, __LINE__);
		return false;
	}
	return true;
}
bool PhysicsEngine::InitFoundation() {
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *mAllocator, *mErrorReporter);
	if (mFoundation == nullptr) {
		mErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating foundation component", __FILE__, __LINE__);
		return false;
	}
	return true;
}
bool PhysicsEngine::InitVisualDebugger() {
	mVisualDebugger = physx::PxCreatePvd(*mFoundation);
	if (!mVisualDebugger) return false;

	physx::PxPvdTransport* pTransport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, PVD_PORT, 10);
	mVisualDebugger->connect(*pTransport, physx::PxPvdInstrumentationFlag::eALL);
	
	return true;
}
bool PhysicsEngine::InitPhysics(const PhysicsSettings& settings) {
	if (settings.CreateFullPhysics()) {
		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, settings.toleranceScale, settings.bTrackMemoryAllocations, mVisualDebugger);
		if (!mPhysics) {
			mErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating physics component", __FILE__, __LINE__);
			return false;
		}
	} 
	else {
		mPhysics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *mFoundation, settings.toleranceScale, settings.bTrackMemoryAllocations, mVisualDebugger);
		if (!mPhysics) {
			mErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating physics component", __FILE__, __LINE__);
			return false;
		}

		if (settings.bEnableHeightFields) PxRegisterHeightFields(*mPhysics);
		if (settings.bEnableArticulations) PxRegisterArticulations(*mPhysics);
		if (settings.bEnableArticulationsReducedCoordinates) PxRegisterArticulationsReducedCoordinate(*mPhysics);
	}
	return true;
}
bool PhysicsEngine::InitCooking(const PhysicsSettings& settings) {
	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, physx::PxCookingParams(settings.toleranceScale));
	if (!mCooking) {
		mErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating cooking component", __FILE__, __LINE__);
		return false;
	}
	return true;
}
bool PhysicsEngine::InitExtensions() {
	bool bSuccess = PxInitExtensions(*mPhysics, mVisualDebugger);
	if (!bSuccess) {
		mErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error Initializing extensions", __FILE__, __LINE__);
		return false;
	}	
	return true;
}
#pragma endregion

bool PhysicsSettings::CreateFullPhysics() const {
	return bEnableArticulations && bEnableArticulationsReducedCoordinates && bEnableHeightFields;
}