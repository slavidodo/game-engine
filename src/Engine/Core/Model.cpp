
#include "pch.h"

#include "Model.h"

#include "../Filesystem/ResourceManager.h"

Model::Model(std::string path, const aiScene* pScene)
{
	mPath = path;
	mParentPath = mPath.substr(0, mPath.find_last_of('/') + 1);
	mFilename = mPath.substr(mParentPath.length(), mPath.length() - 1);
	Load(pScene);
}

Model::~Model()
{
}

void Model::Load(const aiScene* pScene)
{
	ProcessHierarchy(pScene->mRootNode, pScene);
	mModelReady = true;
	// TODO
	// process cameras (attach camera object to the specified node)
	// process lights
	// should this be really done in model loader?

	//std::cout
	//	//<< "        Path: " << mPath.substr(mBasePath.length(), mPath.length())
	//	//<< "\n    BasePath: " << mBasePath
	//	<< "\n      Meshes: " << pScene->mNumMeshes
	//	<< "\n  Animations: " << pScene->mNumAnimations
	//	<< "\n   Materials: " << pScene->mNumMaterials
	//	<< "\n      Lights: " << pScene->mNumLights
	//	<< "\n     Cameras: " << pScene->mNumCameras
	//	<< "\n  Textures_1: " << pScene->mNumTextures << std::endl;
		//<< "\n  Textures_2: " << nn << "\n" << std::endl;
}

void Model::ProcessHierarchy(const aiNode* pNode, const aiScene* pScene, const aiNode* pParentNode /* = nullptr */)
{
	// if pNode is parent, pParentNode = nullptr
	for (size_t i = 0; i < pNode->mNumMeshes; ++i) {
		size_t meshIndex = pNode->mMeshes[i];
		const aiMesh* pMesh = pScene->mMeshes[meshIndex];

		//create mesh
		StaticMesh_ptr mesh = std::make_shared<StaticMesh>();

		auto meshIndices = GetMeshIndices(pMesh);
		auto meshVerts = GetMeshVertices(pMesh);

		StaticMeshCreateInfo createInfo;
		createInfo.NumVertices = meshVerts.size();
		createInfo.NumIndices = meshIndices.size();
		createInfo.Vertices = &meshVerts[0];
		createInfo.Indices = &meshIndices[0];
		createInfo.IndexType = RHIIndexBufferType::IBT_32;

		CreateStaticMeshWithDefaultFilter(mesh, createInfo);
		mStaticMeshList.push_back(mesh);
	}

	//std::cout << (pNode->mName.C_Str()) << std::endl;
	for (size_t childIndex = 0; childIndex < pNode->mNumChildren; childIndex++) {
		const aiNode* pChildNode = pNode->mChildren[childIndex];
		//std::cout << (pChildNode->mName.C_Str()) << " ";
		ProcessHierarchy(pChildNode, pScene, pNode);
	}
}

std::vector<StaticMeshVertex> Model::GetMeshVertices(const aiMesh* pMesh)
{
	std::vector<StaticMeshVertex> vertices(pMesh->mNumVertices);
	for (size_t i = 0; i < pMesh->mNumVertices; i++) {
		StaticMeshVertex vertex;

		if (pMesh->HasPositions()) {
			aiVector3D position = pMesh->mVertices[i];
			vertex.position = glm::fvec3(position.x, position.y, position.z);
		}

		//if (pMesh->HasNormals()) {
		//	aiVector3D normal = pMesh->mNormals[i];
		//	vertex.normal = glm::fvec3(normal.x, normal.y, normal.z);
		//}

		//if (pMesh->HasTangentsAndBitangents()) {
		//	aiVector3D tangent = pMesh->mTangents[i];
		//	aiVector3D bitangent = pMesh->mBitangents[i];
		//	vertex.tangent = glm::fvec3(tangent.x, tangent.y, tangent.z);
		//	vertex.bitangent = glm::fvec3(bitangent.x, bitangent.y, bitangent.z);
		//}

		// todo; support cubemap w component
		if (pMesh->HasTextureCoords(0)) {
			aiVector3D uv = pMesh->mTextureCoords[0][i];
			vertex.uv = glm::fvec2(uv.x, uv.y);
		}

		//if (pMesh->HasTextureCoords(1)) {
		//	aiVector3D uv = pMesh->mTextureCoords[1][i];
		//	vertex.uv1 = glm::fvec2(uv.x, uv.y);
		//}

		if (pMesh->HasVertexColors(0)) {
			aiColor4D color = pMesh->mColors[0][i];
			vertex.color = glm::fvec3(color.r, color.g, color.b); // add color.a by making it vec4 in staticmeshvertix
		}
		else {
			vertex.color = glm::fvec3(1, 0, 1);
		}


		vertices[i] = (vertex);
	}

	return std::move(vertices);
}

std::vector<int32_t> Model::GetMeshIndices(const aiMesh* mesh)
{
	std::vector<int> indices;
	for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
		aiFace face = mesh->mFaces[faceIndex];
		for (uint32_t indexIndex = 0; indexIndex < face.mNumIndices; indexIndex++)
			indices.push_back(face.mIndices[indexIndex]);
	}
	return std::move(indices);
}

void Model::RenderSceneElements(RHICommandList& RHICmdList)
{

	for (const StaticMesh_ptr& Mesh : mStaticMeshList) {
		RHICmdList.SetStreamSource(0, Mesh->VertexBuffer, 0);

		RHICmdList.DrawIndexedPrimitive(Mesh->IndexBuffer, 0, 0, Mesh->NumVertices, 0, Mesh->NumTriangles, 1);
	}
}

StaticMesh_ptr Model::GetFirstMesh()
{
	return mStaticMeshList.front();
}

void Model::CreateStaticMeshWithDefaultFilter(StaticMesh_ptr mesh, StaticMeshCreateInfo& info)
{
	mesh->NumVertices = info.NumVertices;
	mesh->NumIndices = info.NumIndices;
	mesh->NumTriangles = mesh->NumIndices / 3;
	assert(mesh->NumIndices == mesh->NumTriangles * 3);

	void* VerticesPtr = nullptr;
	void* IndicesPtr = nullptr;

	RHICommandListImmediate& RHICommandList = RHICommandListExecutor::GetImmediateCommandList();
	mesh->VertexBuffer = RHICommandList.CreateAndLockVertexBuffer(info.NumVertices,
		sizeof(StaticMeshVertex), // todo change the vertex to use better repr for model
		RHIHardwareBufferUsage::HWBU_STATIC_WRITE_ONLY,
		VerticesPtr);
	mesh->IndexBuffer = RHICommandList.CreateAndLockIndexBuffer(info.NumIndices,
		info.IndexType,
		RHIHardwareBufferUsage::HWBU_STATIC_WRITE_ONLY,
		IndicesPtr);

	memcpy(VerticesPtr, info.Vertices, mesh->VertexBuffer->GetSize());
	memcpy(IndicesPtr, info.Indices, mesh->IndexBuffer->GetSize());
	RHICommandList.UnlockVertexBuffer(mesh->VertexBuffer);
	RHICommandList.UnlockIndexBuffer(mesh->IndexBuffer);
}

/*std::vector<Vertex1P1D1S1M1UV> Model::GetMeshVertices(aiMesh* pMesh)
{
	std::vector<Vertex1P1D1S1M1UV> vertices(pMesh->mNumVertices);
	for (size_t i = 0; i < pMesh->mNumVertices; i++) {
		Vertex1P1D1S1M1UV vertex;

		if (pMesh->HasPositions()) {
			aiVector3D position = pMesh->mVertices[i];
			vertex.position = glm::fvec3(position.x, position.y, position.z);
		}

		if (pMesh->HasNormals()) {
			aiVector3D normal = pMesh->mNormals[i];
			vertex.normal = glm::fvec3(normal.x, normal.y, normal.z);
		}

		if (pMesh->HasTangentsAndBitangents()) {
			aiVector3D tangent = pMesh->mTangents[i];
			aiVector3D bitangent = pMesh->mBitangents[i];
			vertex.tangent = glm::fvec3(tangent.x, tangent.y, tangent.z);
			vertex.bitangent = glm::fvec3(bitangent.x, bitangent.y, bitangent.z);
		}

		// todo; support cubemap w component
		if (pMesh->HasTextureCoords(0)) {
			aiVector3D uv = pMesh->mTextureCoords[0][i];
			vertex.uv0 = glm::fvec2(uv.x, uv.y);
		}

		if (pMesh->HasTextureCoords(1)) {
			aiVector3D uv = pMesh->mTextureCoords[1][i];
			vertex.uv1 = glm::fvec2(uv.x, uv.y);
		}

		if (pMesh->HasVertexColors(0)) {
			aiColor4D color = pMesh->mColors[0][i];
			vertex.diffuse = glm::fvec4(color.r, color.g, color.b, color.a);
		}


		vertices.push_back(vertex);
	}

	return vertices;
}

std::vector<int32_t> Model::GetMeshIndices(aiMesh* mesh)
{
	std::vector<int> indices;
	for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
		aiFace face = mesh->mFaces[faceIndex];
		for (uint32_t indexIndex = 0; indexIndex < face.mNumIndices; indexIndex++)
			indices.push_back(face.mIndices[indexIndex]);
	}
	return std::move(indices);
}

/*std::vector<Texture_ptr> Model::loadMeshTextures(aiMaterial* material, aiTextureType textureType, std::string folderPath)
{
	std::vector<Texture_ptr> textures;
	for (uint32_t textureIndex = 0; textureIndex < material->GetTextureCount(textureType); textureIndex++) {
		// get texture path
		aiString texturePath;
		material->GetTexture(textureType, textureIndex, &texturePath);

		// load the texture
		std::string fullPath = folderPath + texturePath.C_Str();
		textures.push_back(std::move(gResourceManager.loadTexture(fullPath)));
	}

	return std::move(textures);
}*/