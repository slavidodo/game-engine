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

PhysicsEngine* g_pPhysicsEngine;

#pragma region Initializtion/Termination
bool PhysicsEngine::init(const PhysicsSettings& settings) {
	initAllocator(settings);
	initErrorReporter(settings);

	if (!initFoundation())
		return false;
	#ifdef _DEBUG
	if (settings.bEnableVisualDebugger && !initVisualDebugger())
		return false;
	#endif
	if (!initPhysics(settings))
		return false;
	if (!initCooking(settings))
		return false;
	if (!initExtensions())
		return false;

	g_pDefaultMaterial = PMaterial::createMaterial(0.5f, 0.5f, 0.5f);

	return true;
}
void PhysicsEngine::terminate() {
	g_pDefaultMaterial.reset();

	PxCloseExtensions();
	
	if (m_pCooking)
		m_pCooking->release();
	if (m_pPhysics)
		m_pPhysics->release();
	#ifdef _DEBUG
	if (m_pVisualDebugger)
		m_pVisualDebugger->release();
	#endif
	if (m_pFoundation)
		m_pFoundation->release();
	
	PAlignedAllocator::deallocate(m_pErrorReporter);
	PAlignedAllocator::deallocate(m_pAllocator);
}

bool PhysicsEngine::initAllocator(const PhysicsSettings& settings) {
	m_pAllocator = new (PAlignedAllocator::allocate<SdkAllocator>()) SdkAllocator();
	if (!m_pAllocator) {
		m_pErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating allocator component", __FILE__, __LINE__);
		return false;
	}
	return true;
}
bool PhysicsEngine::initErrorReporter(const PhysicsSettings& settings) {
	m_pErrorReporter = new (PAlignedAllocator::allocate<physx::PxDefaultErrorCallback>()) physx::PxDefaultErrorCallback();
	if (!m_pErrorReporter) {
		m_pErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating error callback component", __FILE__, __LINE__);
		return false;
	}
	return true;
}
bool PhysicsEngine::initFoundation() {
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_pAllocator, *m_pErrorReporter);
	if (m_pFoundation == nullptr) {
		m_pErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating foundation component", __FILE__, __LINE__);
		return false;
	}
	return true;
}
bool PhysicsEngine::initVisualDebugger() {
	m_pVisualDebugger = physx::PxCreatePvd(*m_pFoundation);
	if (!m_pVisualDebugger) return false;

	physx::PxPvdTransport* pTransport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, PVD_PORT, 10);
	m_pVisualDebugger->connect(*pTransport, physx::PxPvdInstrumentationFlag::eALL);
	
	return true;
}
bool PhysicsEngine::initPhysics(const PhysicsSettings& settings) {
	if (settings.CreateFullPhysics()) {
		m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, settings.toleranceScale, settings.bTrackMemoryAllocations, m_pVisualDebugger);
		if (!m_pPhysics) {
			m_pErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating physics component", __FILE__, __LINE__);
			return false;
		}
	} 
	else {
		m_pPhysics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *m_pFoundation, settings.toleranceScale, settings.bTrackMemoryAllocations, m_pVisualDebugger);
		if (!m_pPhysics) {
			m_pErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating physics component", __FILE__, __LINE__);
			return false;
		}

		if (settings.bEnableHeightFields) PxRegisterHeightFields(*m_pPhysics);
		if (settings.bEnableArticulations) PxRegisterArticulations(*m_pPhysics);
		if (settings.bEnableArticulationsReducedCoordinates) PxRegisterArticulationsReducedCoordinate(*m_pPhysics);
	}
	return true;
}
bool PhysicsEngine::initCooking(const PhysicsSettings& settings) {
	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxCookingParams(settings.toleranceScale));
	if (!m_pCooking) {
		m_pErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error creating cooking component", __FILE__, __LINE__);
		return false;
	}
	return true;
}
bool PhysicsEngine::initExtensions() {
	bool bSuccess = PxInitExtensions(*m_pPhysics, m_pVisualDebugger);
	if (!bSuccess) {
		m_pErrorReporter->reportError(physx::PxErrorCode::eABORT, "Error initializing extensions", __FILE__, __LINE__);
		return false;
	}	
	return true;
}
#pragma endregion

bool PhysicsSettings::CreateFullPhysics() const {
	return bEnableArticulations && bEnableArticulationsReducedCoordinates && bEnableHeightFields;
}