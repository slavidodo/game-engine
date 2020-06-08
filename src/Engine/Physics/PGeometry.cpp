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

#pragma region Geometry Functions
PGeometry::~PGeometry() {
	PAlignedAllocator::deallocate(m_pGeometry);
}

physx::PxGeometry* PGeometry::getSdkGeometry() const { 
	return m_pGeometry;
}
#pragma endregion

#pragma region Box Geometry Functions
PBoxGeometry::PBoxGeometry(physx::PxBoxGeometry* pBoxGeometry) : m_pBoxGeometry(pBoxGeometry) {
	m_pGeometry = static_cast<physx::PxGeometry*>(m_pBoxGeometry);
}

PBoxGeometry_ptr PBoxGeometry::createGeometry(glm::vec3 halfDimensions) {
	physx::PxBoxGeometry* pBoxGeometry = new (PAlignedAllocator::allocate<physx::PxBoxGeometry>()) physx::PxBoxGeometry(halfDimensions.x, halfDimensions.y, halfDimensions.z);
	PBoxGeometry_ptr geometry = std::make_shared<PBoxGeometry>(pBoxGeometry);
	
	return std::move(geometry);
}
#pragma endregion

#pragma region Sphere Geometry Functions
PSphereGeometry::PSphereGeometry(physx::PxSphereGeometry* pSphereGeometry) : m_pSphereGeometry(pSphereGeometry) {
	m_pGeometry = static_cast<physx::PxGeometry*>(m_pSphereGeometry);
}

PSphereGeometry_ptr PSphereGeometry::createGeometry(float radius) {
	physx::PxSphereGeometry* pSphereGeometry = new (PAlignedAllocator::allocate<physx::PxSphereGeometry>()) physx::PxSphereGeometry(radius);

	PSphereGeometry_ptr geometry = std::make_shared<PSphereGeometry>(pSphereGeometry);

	return std::move(geometry);
}
#pragma endregion

#pragma region Plane Geometry Functions
PPlaneGeometry::PPlaneGeometry(physx::PxPlaneGeometry* pPlaneGeometry) : m_pPlaneGeometry(pPlaneGeometry) {
	m_pGeometry = static_cast<physx::PxGeometry*>(m_pPlaneGeometry);
}

PPlaneGeometry_ptr PPlaneGeometry::createGeometry() {
	physx::PxPlaneGeometry* pPlaneGeometry = new (PAlignedAllocator::allocate<physx::PxPlaneGeometry>()) physx::PxPlaneGeometry();

	PPlaneGeometry_ptr geometry = std::make_shared<PPlaneGeometry>(pPlaneGeometry);

	return std::move(geometry);
}
#pragma endregion

#pragma region Capsule Geometry Functions
PCapsuleGeometry::PCapsuleGeometry(physx::PxCapsuleGeometry* pCapsuleGeometry) : m_pCapsuleGeometry(pCapsuleGeometry) {
	m_pGeometry = static_cast<physx::PxGeometry*>(m_pCapsuleGeometry);
}

PCapsuleGeometry_ptr PCapsuleGeometry::createGeometry(float radius, float halfHeight) {
	physx::PxCapsuleGeometry* pCapsuleGeometry = new (PAlignedAllocator::allocate<physx::PxCapsuleGeometry>()) physx::PxCapsuleGeometry(radius, halfHeight);

	PCapsuleGeometry_ptr geometry = std::make_shared<PCapsuleGeometry>(pCapsuleGeometry);

	return std::move(geometry);
}
#pragma endregion

#pragma region Convex Mesh Descriptor Functions
PConvexMeshDescriptor::PConvexMeshDescriptor() {
	m_pDescriptor = new (PAlignedAllocator::allocate<physx::PxConvexMeshDesc>()) physx::PxConvexMeshDesc();
}
PConvexMeshDescriptor::~PConvexMeshDescriptor() {
	PAlignedAllocator::deallocate(m_pDescriptor);
}

const physx::PxConvexMeshDesc* PConvexMeshDescriptor::getSdkDescriptor() const {
	return m_pDescriptor;
}

void PConvexMeshDescriptor::setPointsCount(uint32_t count) {
	m_pDescriptor->points.count = count;
}
void PConvexMeshDescriptor::setPointsStride(size_t stride) {
	m_pDescriptor->points.stride = stride;
}
void PConvexMeshDescriptor::setPointsData(float* data) {
	m_pDescriptor->points.data = data;
}

void PConvexMeshDescriptor::enableAutomaticHullComputation() {
	m_pDescriptor->flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
}
void PConvexMeshDescriptor::disableMeshValidation() {
	m_pDescriptor->flags |= physx::PxConvexFlag::eDISABLE_MESH_VALIDATION;
}
void PConvexMeshDescriptor::enableFastInertiaComputation() {
	m_pDescriptor->flags |= physx::PxConvexFlag::eFAST_INERTIA_COMPUTATION;
}
#pragma endregion
#pragma region Convex Mesh Geometry Functions
PConvexMeshGeometry::PConvexMeshGeometry(physx::PxConvexMeshGeometry* pConvexMeshGeometry) : m_pConvexGeometry(pConvexMeshGeometry) {
	m_pGeometry = static_cast<physx::PxGeometry*>(m_pConvexGeometry);
}

PConvexMeshGeometry_ptr PConvexMeshGeometry::createGeometry(const PConvexMeshDescriptor& descriptor) {
	const physx::PxConvexMeshDesc* desc = descriptor.getSdkDescriptor();

	/// Cook convex mesh
	physx::PxDefaultMemoryOutputStream tempBuffer;
	if (!g_pPhysicsEngine->m_pCooking->cookConvexMesh(*desc, tempBuffer)) return nullptr;

	/// Create convex mesh
	physx::PxDefaultMemoryInputData meshInputBuffer(tempBuffer.getData(), tempBuffer.getSize());
	physx::PxConvexMesh* pConvexMesh = g_pPhysicsEngine->m_pPhysics->createConvexMesh(meshInputBuffer);

	/// Create convex mesh geometry
	physx::PxConvexMeshGeometry* pConvexGeometry = new (PAlignedAllocator::allocate<physx::PxConvexMeshGeometry>()) physx::PxConvexMeshGeometry(pConvexMesh);

	PConvexMeshGeometry_ptr geometry = std::make_shared<PConvexMeshGeometry>(pConvexGeometry);
	
	return std::move(geometry);
}
#pragma endregion

#pragma region Triangle Mesh Descriptor Functions
PTriangleMeshDescriptor::PTriangleMeshDescriptor() {
	m_pDescriptor = new (PAlignedAllocator::allocate<physx::PxTriangleMeshDesc>()) physx::PxTriangleMeshDesc();
}
PTriangleMeshDescriptor::~PTriangleMeshDescriptor() {
	PAlignedAllocator::deallocate(m_pDescriptor);
}

const physx::PxTriangleMeshDesc* PTriangleMeshDescriptor::getSdkDescriptor() const {
	return m_pDescriptor;
}

void PTriangleMeshDescriptor::setPointsCount(uint32_t count) {
	m_pDescriptor->points.count = count;
}
void PTriangleMeshDescriptor::setPointsStride(size_t stride) {
	m_pDescriptor->points.stride = stride;
}
void PTriangleMeshDescriptor::setPointsData(float* data) {
	m_pDescriptor->points.data = data;
}

void PTriangleMeshDescriptor::setTrianglesCount(uint32_t count) {
	m_pDescriptor->triangles.count = count;
}
void PTriangleMeshDescriptor::setTrianglesStride(size_t stride) {
	m_pDescriptor->triangles.stride = stride;
}
void PTriangleMeshDescriptor::setTrianglesData(uint32_t* data) {
	m_pDescriptor->triangles.data = data;
}
#pragma endregion
#pragma region Triangle Mesh Geometry Functions
PTriangleMeshGeometry::PTriangleMeshGeometry(physx::PxTriangleMeshGeometry* pTriangleMeshGeometry) : m_pTriangleGeometry(pTriangleMeshGeometry) {
	m_pGeometry = static_cast<physx::PxGeometry*>(m_pTriangleGeometry);
}

PTriangleMeshGeometry_ptr PTriangleMeshGeometry::createGeometry(const PTriangleMeshDescriptor& descriptor) {
	/// Cook Triangle mesh
	physx::PxDefaultMemoryOutputStream tempBuffer;
	if (!g_pPhysicsEngine->m_pCooking->cookTriangleMesh(*descriptor.getSdkDescriptor(), tempBuffer)) return nullptr;

	/// Create Triangle mesh
	physx::PxDefaultMemoryInputData meshInputBuffer(tempBuffer.getData(), tempBuffer.getSize());
	physx::PxTriangleMesh* pTriangleMesh = g_pPhysicsEngine->m_pPhysics->createTriangleMesh(meshInputBuffer);

	/// Create Triangle mesh geometry
	physx::PxTriangleMeshGeometry* pTriangleGeometry = new (PAlignedAllocator::allocate<physx::PxTriangleMeshGeometry>()) physx::PxTriangleMeshGeometry(pTriangleMesh);
	PTriangleMeshGeometry_ptr geometry = std::make_shared<PTriangleMeshGeometry>(pTriangleGeometry);

	return std::move(geometry);
}
#pragma endregion

#pragma region Height Field Structure Functions
PHeightFieldStructure::PHeightFieldStructure(uint16_t numOfRows, uint16_t numOfCols, float heightScale, float rowScale, float colScale) : m_numOfRows(numOfRows), m_numOfCols(numOfCols), m_heightScale(heightScale), m_rowScale(rowScale), m_colScale(colScale) {
	m_pVertices = new (PAlignedAllocator::allocate<physx::PxHeightFieldSample>(m_numOfRows * m_numOfCols)) physx::PxHeightFieldSample();
}
PHeightFieldStructure::~PHeightFieldStructure() {
	PAlignedAllocator::deallocate(m_pVertices);
}

physx::PxHeightFieldSample* PHeightFieldStructure::getSdkSamples() const {
	return m_pVertices;
}

uint16_t PHeightFieldStructure::getNumOfRows() const {
	return m_numOfRows;
}
uint16_t PHeightFieldStructure::getNumOfCols() const {
	return m_numOfCols;
}
float PHeightFieldStructure::getHeightScale() const {
	return m_heightScale;
}
float PHeightFieldStructure::getRowScale() const {
	return m_rowScale;
}
float PHeightFieldStructure::getColScale() const {
	return m_colScale;
}

bool PHeightFieldStructure::isValidIndex(uint16_t row, uint16_t col) const {
	if (row < 0 || col < 0 || row >= m_numOfRows || col >= m_numOfCols) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Height field index out of bounds", __FILE__, __LINE__);
		return false;
	}
	return true;
}
void PHeightFieldStructure::setVertexHeight(uint16_t row, uint16_t col, uint16_t height) {
	if (!isValidIndex(row, col)) return;
	m_pVertices[row * m_numOfCols + col].height = height;
}
void PHeightFieldStructure::setVertexMaterials(uint16_t row, uint16_t col, uint16_t materialIndex1, uint16_t materialIndex2) {
	if (!isValidIndex(row, col)) return;
	m_pVertices[row * m_numOfCols + col].materialIndex0 = 0;
	m_pVertices[row * m_numOfCols + col].materialIndex1 = 0;
}
#pragma endregion
#pragma region Height Field Descriptor Functions
PHeightFieldDescriptor::PHeightFieldDescriptor(const PHeightFieldStructure& structure) : m_heightScale(structure.getHeightScale()), m_rowScale(structure.getRowScale()), m_colScale(structure.getColScale()) {
	m_pDescriptor = new (PAlignedAllocator::allocate<physx::PxHeightFieldDesc>()) physx::PxHeightFieldDesc();
	
	m_pDescriptor->format = physx::PxHeightFieldFormat::eS16_TM;
	m_pDescriptor->nbRows = structure.getNumOfRows();
	m_pDescriptor->nbColumns = structure.getNumOfCols();
	m_pDescriptor->samples.data = structure.getSdkSamples();
	m_pDescriptor->samples.stride = sizeof(physx::PxHeightFieldSample);

	if (!m_pDescriptor->isValid()) {
		g_pPhysicsEngine->m_pErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Height field descriptor invalid", __FILE__, __LINE__);
		return;
	}
}
PHeightFieldDescriptor::~PHeightFieldDescriptor() {
	PAlignedAllocator::deallocate(m_pDescriptor);
}

const physx::PxHeightFieldDesc* PHeightFieldDescriptor::getSdkDescriptor() const {
	return m_pDescriptor;
}
#pragma endregion
#pragma region Height Field Geometry Functions
PHeightFieldGeometry::PHeightFieldGeometry(physx::PxHeightFieldGeometry* pHeightFieldGeometry) : m_pHeightFieldGeometry(pHeightFieldGeometry) {
	m_pGeometry = static_cast<physx::PxGeometry*>(m_pHeightFieldGeometry);
}

PHeightFieldGeometry_ptr PHeightFieldGeometry::createGeometry(const PHeightFieldDescriptor& descriptor) {
	/// Cook height field
	physx::PxDefaultMemoryOutputStream tempBuffer;
	if (!g_pPhysicsEngine->m_pCooking->cookHeightField(*descriptor.getSdkDescriptor(), tempBuffer)) return nullptr;

	/// Create height field
	physx::PxDefaultMemoryInputData meshInputBuffer(tempBuffer.getData(), tempBuffer.getSize());
	physx::PxHeightField* pHeightField = g_pPhysicsEngine->m_pPhysics->createHeightField(meshInputBuffer);

	/// Create height field geometry
	physx::PxHeightFieldGeometry* pHeightFieldGeometry = new (PAlignedAllocator::allocate<physx::PxHeightFieldGeometry>()) physx::PxHeightFieldGeometry(pHeightField, physx::PxMeshGeometryFlags(), descriptor.m_heightScale, descriptor.m_rowScale, descriptor.m_colScale);

	PHeightFieldGeometry_ptr geometry = std::make_shared<PHeightFieldGeometry>(pHeightFieldGeometry);

	return std::move(geometry);
}
#pragma endregion

