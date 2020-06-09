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
	PAlignedAllocator::deallocate(mGeometry);
}

physx::PxGeometry* PGeometry::GetSdkGeometry() const { 
	return mGeometry;
}
#pragma endregion

#pragma region Box Geometry Functions
PBoxGeometry::PBoxGeometry(physx::PxBoxGeometry* pBoxGeometry) : mBoxGeometry(pBoxGeometry) {
	mGeometry = static_cast<physx::PxGeometry*>(mBoxGeometry);
}

PBoxGeometry_ptr PBoxGeometry::CreateGeometry(glm::vec3 halfDimensions) {
	physx::PxBoxGeometry* pBoxGeometry = new (PAlignedAllocator::allocate<physx::PxBoxGeometry>()) physx::PxBoxGeometry(halfDimensions.x, halfDimensions.y, halfDimensions.z);
	PBoxGeometry_ptr geometry = std::make_shared<PBoxGeometry>(pBoxGeometry);
	
	return std::move(geometry);
}
#pragma endregion

#pragma region Sphere Geometry Functions
PSphereGeometry::PSphereGeometry(physx::PxSphereGeometry* pSphereGeometry) : mSphereGeometry(pSphereGeometry) {
	mGeometry = static_cast<physx::PxGeometry*>(mSphereGeometry);
}

PSphereGeometry_ptr PSphereGeometry::CreateGeometry(float radius) {
	physx::PxSphereGeometry* pSphereGeometry = new (PAlignedAllocator::allocate<physx::PxSphereGeometry>()) physx::PxSphereGeometry(radius);

	PSphereGeometry_ptr geometry = std::make_shared<PSphereGeometry>(pSphereGeometry);

	return std::move(geometry);
}
#pragma endregion

#pragma region Plane Geometry Functions
PPlaneGeometry::PPlaneGeometry(physx::PxPlaneGeometry* pPlaneGeometry) : mPlaneGeometry(pPlaneGeometry) {
	mGeometry = static_cast<physx::PxGeometry*>(mPlaneGeometry);
}

PPlaneGeometry_ptr PPlaneGeometry::CreateGeometry() {
	physx::PxPlaneGeometry* pPlaneGeometry = new (PAlignedAllocator::allocate<physx::PxPlaneGeometry>()) physx::PxPlaneGeometry();

	PPlaneGeometry_ptr geometry = std::make_shared<PPlaneGeometry>(pPlaneGeometry);

	return std::move(geometry);
}
#pragma endregion

#pragma region Capsule Geometry Functions
PCapsuleGeometry::PCapsuleGeometry(physx::PxCapsuleGeometry* pCapsuleGeometry) : mCapsuleGeometry(pCapsuleGeometry) {
	mGeometry = static_cast<physx::PxGeometry*>(mCapsuleGeometry);
}

PCapsuleGeometry_ptr PCapsuleGeometry::CreateGeometry(float radius, float halfHeight) {
	physx::PxCapsuleGeometry* pCapsuleGeometry = new (PAlignedAllocator::allocate<physx::PxCapsuleGeometry>()) physx::PxCapsuleGeometry(radius, halfHeight);

	PCapsuleGeometry_ptr geometry = std::make_shared<PCapsuleGeometry>(pCapsuleGeometry);

	return std::move(geometry);
}
#pragma endregion

#pragma region Convex Mesh Descriptor Functions
PConvexMeshDescriptor::PConvexMeshDescriptor() {
	mDescriptor = new (PAlignedAllocator::allocate<physx::PxConvexMeshDesc>()) physx::PxConvexMeshDesc();
}
PConvexMeshDescriptor::~PConvexMeshDescriptor() {
	PAlignedAllocator::deallocate(mDescriptor);
}

const physx::PxConvexMeshDesc* PConvexMeshDescriptor::GetSdkDescriptor() const {
	return mDescriptor;
}

void PConvexMeshDescriptor::SetPointsCount(uint32_t count) {
	mDescriptor->points.count = count;
}
void PConvexMeshDescriptor::SetPointsStride(size_t stride) {
	mDescriptor->points.stride = stride;
}
void PConvexMeshDescriptor::SetPointsData(float* data) {
	mDescriptor->points.data = data;
}

void PConvexMeshDescriptor::EnableAutomaticHullComputation() {
	mDescriptor->flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
}
void PConvexMeshDescriptor::DisableMeshValidation() {
	mDescriptor->flags |= physx::PxConvexFlag::eDISABLE_MESH_VALIDATION;
}
void PConvexMeshDescriptor::EnableFastInertiaComputation() {
	mDescriptor->flags |= physx::PxConvexFlag::eFAST_INERTIA_COMPUTATION;
}
#pragma endregion
#pragma region Convex Mesh Geometry Functions
PConvexMeshGeometry::PConvexMeshGeometry(physx::PxConvexMeshGeometry* pConvexMeshGeometry) : mConvexGeometry(pConvexMeshGeometry) {
	mGeometry = static_cast<physx::PxGeometry*>(mConvexGeometry);
}

PConvexMeshGeometry_ptr PConvexMeshGeometry::CreateGeometry(const PConvexMeshDescriptor& descriptor) {
	const physx::PxConvexMeshDesc* desc = descriptor.GetSdkDescriptor();

	/// Cook convex mesh
	physx::PxDefaultMemoryOutputStream tempBuffer;
	if (!PhysicsEngine::GetInstance().mCooking->cookConvexMesh(*desc, tempBuffer)) return nullptr;

	/// Create convex mesh
	physx::PxDefaultMemoryInputData meshInputBuffer(tempBuffer.getData(), tempBuffer.getSize());
	physx::PxConvexMesh* pConvexMesh = PhysicsEngine::GetInstance().mPhysics->createConvexMesh(meshInputBuffer);

	/// Create convex mesh geometry
	physx::PxConvexMeshGeometry* pConvexGeometry = new (PAlignedAllocator::allocate<physx::PxConvexMeshGeometry>()) physx::PxConvexMeshGeometry(pConvexMesh);

	PConvexMeshGeometry_ptr geometry = std::make_shared<PConvexMeshGeometry>(pConvexGeometry);
	
	return std::move(geometry);
}
#pragma endregion

#pragma region Triangle Mesh Descriptor Functions
PTriangleMeshDescriptor::PTriangleMeshDescriptor() {
	mDescriptor = new (PAlignedAllocator::allocate<physx::PxTriangleMeshDesc>()) physx::PxTriangleMeshDesc();
}
PTriangleMeshDescriptor::~PTriangleMeshDescriptor() {
	PAlignedAllocator::deallocate(mDescriptor);
}

const physx::PxTriangleMeshDesc* PTriangleMeshDescriptor::GetSdkDescriptor() const {
	return mDescriptor;
}

void PTriangleMeshDescriptor::SetPointsCount(uint32_t count) {
	mDescriptor->points.count = count;
}
void PTriangleMeshDescriptor::SetPointsStride(size_t stride) {
	mDescriptor->points.stride = stride;
}
void PTriangleMeshDescriptor::SetPointsData(float* data) {
	mDescriptor->points.data = data;
}

void PTriangleMeshDescriptor::SetTrianglesCount(uint32_t count) {
	mDescriptor->triangles.count = count;
}
void PTriangleMeshDescriptor::SetTrianglesStride(size_t stride) {
	mDescriptor->triangles.stride = stride;
}
void PTriangleMeshDescriptor::SetTrianglesData(uint32_t* data) {
	mDescriptor->triangles.data = data;
}
#pragma endregion
#pragma region Triangle Mesh Geometry Functions
PTriangleMeshGeometry::PTriangleMeshGeometry(physx::PxTriangleMeshGeometry* pTriangleMeshGeometry) : mTriangleGeometry(pTriangleMeshGeometry) {
	mGeometry = static_cast<physx::PxGeometry*>(mTriangleGeometry);
}

PTriangleMeshGeometry_ptr PTriangleMeshGeometry::CreateGeometry(const PTriangleMeshDescriptor& descriptor) {
	/// Cook Triangle mesh
	physx::PxDefaultMemoryOutputStream tempBuffer;
	if (!PhysicsEngine::GetInstance().mCooking->cookTriangleMesh(*descriptor.GetSdkDescriptor(), tempBuffer)) return nullptr;

	/// Create Triangle mesh
	physx::PxDefaultMemoryInputData meshInputBuffer(tempBuffer.getData(), tempBuffer.getSize());
	physx::PxTriangleMesh* pTriangleMesh = PhysicsEngine::GetInstance().mPhysics->createTriangleMesh(meshInputBuffer);

	/// Create Triangle mesh geometry
	physx::PxTriangleMeshGeometry* pTriangleGeometry = new (PAlignedAllocator::allocate<physx::PxTriangleMeshGeometry>()) physx::PxTriangleMeshGeometry(pTriangleMesh);
	PTriangleMeshGeometry_ptr geometry = std::make_shared<PTriangleMeshGeometry>(pTriangleGeometry);

	return std::move(geometry);
}
#pragma endregion

#pragma region Height Field Structure Functions
PHeightFieldStructure::PHeightFieldStructure(uint16_t numOfRows, uint16_t numOfCols, float heightScale, float rowScale, float colScale) : mNumOfRows(numOfRows), mNumOfCols(numOfCols), mHeightScale(heightScale), mRowScale(rowScale), mColScale(colScale) {
	mVertices = new (PAlignedAllocator::allocate<physx::PxHeightFieldSample>(mNumOfRows * mNumOfCols)) physx::PxHeightFieldSample();
}
PHeightFieldStructure::~PHeightFieldStructure() {
	PAlignedAllocator::deallocate(mVertices);
}

physx::PxHeightFieldSample* PHeightFieldStructure::GetSdkSamples() const {
	return mVertices;
}

uint16_t PHeightFieldStructure::GetNumOfRows() const {
	return mNumOfRows;
}
uint16_t PHeightFieldStructure::GetNumOfCols() const {
	return mNumOfCols;
}
float PHeightFieldStructure::GetHeightScale() const {
	return mHeightScale;
}
float PHeightFieldStructure::GetRowScale() const {
	return mRowScale;
}
float PHeightFieldStructure::GetColScale() const {
	return mColScale;
}

bool PHeightFieldStructure::isValidIndex(uint16_t row, uint16_t col) const {
	if (row < 0 || col < 0 || row >= mNumOfRows || col >= mNumOfCols) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Height field index out of bounds", __FILE__, __LINE__);
		return false;
	}
	return true;
}
void PHeightFieldStructure::SetVertexHeight(uint16_t row, uint16_t col, uint16_t height) {
	if (!isValidIndex(row, col)) return;
	mVertices[row * mNumOfCols + col].height = height;
}
void PHeightFieldStructure::SetVertexMaterials(uint16_t row, uint16_t col, uint16_t materialIndex1, uint16_t materialIndex2) {
	if (!isValidIndex(row, col)) return;
	mVertices[row * mNumOfCols + col].materialIndex0 = 0;
	mVertices[row * mNumOfCols + col].materialIndex1 = 0;
}
#pragma endregion
#pragma region Height Field Descriptor Functions
PHeightFieldDescriptor::PHeightFieldDescriptor(const PHeightFieldStructure& structure) : mHeightScale(structure.GetHeightScale()), mRowScale(structure.GetRowScale()), mColScale(structure.GetColScale()) {
	mDescriptor = new (PAlignedAllocator::allocate<physx::PxHeightFieldDesc>()) physx::PxHeightFieldDesc();
	
	mDescriptor->format = physx::PxHeightFieldFormat::eS16_TM;
	mDescriptor->nbRows = structure.GetNumOfRows();
	mDescriptor->nbColumns = structure.GetNumOfCols();
	mDescriptor->samples.data = structure.GetSdkSamples();
	mDescriptor->samples.stride = sizeof(physx::PxHeightFieldSample);

	if (!mDescriptor->isValid()) {
		PhysicsEngine::GetInstance().mErrorReporter->reportError(physx::PxErrorCode::eDEBUG_WARNING, "Height field descriptor invalid", __FILE__, __LINE__);
		return;
	}
}
PHeightFieldDescriptor::~PHeightFieldDescriptor() {
	PAlignedAllocator::deallocate(mDescriptor);
}

const physx::PxHeightFieldDesc* PHeightFieldDescriptor::GetSdkDescriptor() const {
	return mDescriptor;
}
#pragma endregion
#pragma region Height Field Geometry Functions
PHeightFieldGeometry::PHeightFieldGeometry(physx::PxHeightFieldGeometry* pHeightFieldGeometry) : mHeightFieldGeometry(pHeightFieldGeometry) {
	mGeometry = static_cast<physx::PxGeometry*>(mHeightFieldGeometry);
}

PHeightFieldGeometry_ptr PHeightFieldGeometry::CreateGeometry(const PHeightFieldDescriptor& descriptor) {
	/// Cook height field
	physx::PxDefaultMemoryOutputStream tempBuffer;
	if (!PhysicsEngine::GetInstance().mCooking->cookHeightField(*descriptor.GetSdkDescriptor(), tempBuffer)) return nullptr;

	/// Create height field
	physx::PxDefaultMemoryInputData meshInputBuffer(tempBuffer.getData(), tempBuffer.getSize());
	physx::PxHeightField* pHeightField = PhysicsEngine::GetInstance().mPhysics->createHeightField(meshInputBuffer);

	/// Create height field geometry
	physx::PxHeightFieldGeometry* pHeightFieldGeometry = new (PAlignedAllocator::allocate<physx::PxHeightFieldGeometry>()) physx::PxHeightFieldGeometry(pHeightField, physx::PxMeshGeometryFlags(), descriptor.mHeightScale, descriptor.mRowScale, descriptor.mColScale);

	PHeightFieldGeometry_ptr geometry = std::make_shared<PHeightFieldGeometry>(pHeightFieldGeometry);

	return std::move(geometry);
}
#pragma endregion

