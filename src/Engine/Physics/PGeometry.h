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

class PGeometry;
class PBoxGeometry;
class PPlaneGeometry;
class PSphereGeometry;
class PCapsuleGeometry;
class PConvexMeshGeometry;
class PTriangleMeshGeometry;
class PHeightFieldGeometry;

typedef std::shared_ptr<PGeometry> PGeometry_ptr;
typedef std::shared_ptr<PBoxGeometry> PBoxGeometry_ptr;
typedef std::shared_ptr<PPlaneGeometry> PPlaneGeometry_ptr;
typedef std::shared_ptr<PSphereGeometry> PSphereGeometry_ptr;
typedef std::shared_ptr<PCapsuleGeometry> PCapsuleGeometry_ptr;
typedef std::shared_ptr<PConvexMeshGeometry> PConvexMeshGeometry_ptr;
typedef std::shared_ptr<PTriangleMeshGeometry> PTriangleMeshGeometry_ptr;
typedef std::shared_ptr<PHeightFieldGeometry> PHeightFieldGeometry_ptr;

class PGeometry {
public:
	~PGeometry();
	physx::PxGeometry* GetSdkGeometry() const;

protected:
	physx::PxGeometry* mGeometry = nullptr;
};

#pragma region Simple Primitives
class PBoxGeometry : public PGeometry {
public:
	PBoxGeometry(physx::PxBoxGeometry* pGeometry);
	static PBoxGeometry_ptr CreateGeometry(glm::vec3 halfDimensions = glm::vec3(1.0f));
	
private:
	// For specific operations
	physx::PxBoxGeometry* mBoxGeometry = nullptr;
};
class PSphereGeometry : public PGeometry {
public:
	PSphereGeometry(physx::PxSphereGeometry* pGeometry);
	static PSphereGeometry_ptr CreateGeometry(float radius = 1.0f);
	
private:
	// For specific operations
	physx::PxSphereGeometry* mSphereGeometry = nullptr;
};
class PPlaneGeometry : public PGeometry {
public:
	PPlaneGeometry(physx::PxPlaneGeometry* pGeometry);
	static PPlaneGeometry_ptr CreateGeometry();

private:
	// For specific operations
	physx::PxPlaneGeometry* mPlaneGeometry = nullptr;
};
class PCapsuleGeometry : public PGeometry {
public:
	PCapsuleGeometry(physx::PxCapsuleGeometry* pGeometry);
	static PCapsuleGeometry_ptr CreateGeometry(float radius = 1.0f, float halfHeight = 2.0f);

private:
	// For specific operations
	physx::PxCapsuleGeometry* mCapsuleGeometry = nullptr;
};
#pragma endregion

#pragma region Complex Meshes
struct PConvexMeshDescriptor {
public:
	PConvexMeshDescriptor();
	~PConvexMeshDescriptor();

	const physx::PxConvexMeshDesc* GetSdkDescriptor() const;

	// mandatory
	void SetPointsCount(uint32_t count);
	// mandatory
	void SetPointsStride(size_t stride);
	// mandatory
	void SetPointsData(float* data);

	// increase performance at the cost of safety
	void DisableMeshValidation();
	// increase performace at the cost of precision
	void EnableFastInertiaComputation();
	// should be enabled if only vertices are provided (no face data)
	void EnableAutomaticHullComputation();
	

private:
	physx::PxConvexMeshDesc* mDescriptor = nullptr;
};
class PConvexMeshGeometry : public PGeometry {
public:
	PConvexMeshGeometry(physx::PxConvexMeshGeometry* pConvexMeshGeometry);
	static PConvexMeshGeometry_ptr CreateGeometry(const PConvexMeshDescriptor& descriptor);

private:
	physx::PxConvexMeshGeometry* mConvexGeometry = nullptr;
};

struct PTriangleMeshDescriptor {
public:
	PTriangleMeshDescriptor();
	~PTriangleMeshDescriptor();

	const physx::PxTriangleMeshDesc* GetSdkDescriptor() const;

	// mandatory
	void SetPointsCount(uint32_t count);
	// mandatory
	void SetPointsStride(size_t stride);
	// mandatory
	void SetPointsData(float* data);

	// mandatory
	void SetTrianglesCount(uint32_t count);
	// mandatory
	void SetTrianglesStride(size_t stride);
	// mandatory
	void SetTrianglesData(uint32_t* data);


private:
	physx::PxTriangleMeshDesc* mDescriptor = nullptr;
};
class PTriangleMeshGeometry : public PGeometry {
public:
	PTriangleMeshGeometry(physx::PxTriangleMeshGeometry* pTriangleMeshGeometry);
	static PTriangleMeshGeometry_ptr CreateGeometry(const PTriangleMeshDescriptor& descriptor);

private:
	physx::PxTriangleMeshGeometry* mTriangleGeometry = nullptr;
};

struct PHeightFieldStructure {
	PHeightFieldStructure(uint16_t numOfRows, uint16_t numOfCols, float heightScale = 1.0f, float rowScale = 1.0f, float colScale = 1.0f);
	~PHeightFieldStructure();

	physx::PxHeightFieldSample* GetSdkSamples() const;

	uint16_t GetNumOfRows() const;
	uint16_t GetNumOfCols() const;
	float GetRowScale() const;
	float GetColScale() const;
	float GetHeightScale() const;

	void SetVertexHeight(uint16_t row, uint16_t col, uint16_t height);
	void SetVertexMaterials(uint16_t row, uint16_t col, uint16_t materialIndex1, uint16_t materialIndex2);

private:
	uint16_t mNumOfRows = 0, mNumOfCols = 0;
	float mHeightScale, mRowScale, mColScale;
	physx::PxHeightFieldSample* mVertices = nullptr;

	bool isValidIndex(uint16_t row, uint16_t col) const;
};
struct PHeightFieldDescriptor {
public:
	PHeightFieldDescriptor(const PHeightFieldStructure& structure);
	~PHeightFieldDescriptor();

	const physx::PxHeightFieldDesc* GetSdkDescriptor() const;
	float mHeightScale, mRowScale, mColScale;
		
private:
	physx::PxHeightFieldDesc* mDescriptor = nullptr;
};
class PHeightFieldGeometry : public PGeometry {
public:
	PHeightFieldGeometry(physx::PxHeightFieldGeometry* pHeightFieldGeometry);
	static PHeightFieldGeometry_ptr CreateGeometry(const PHeightFieldDescriptor& descriptor);

private:
	physx::PxHeightFieldGeometry* mHeightFieldGeometry = nullptr;
};
#pragma endregion