
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

	// TODO
	// process cameras (attach camera object to the specified node)
	// process lights

	//std::cout
	//	<< "        Path: " << mPath.substr(mBasePath.length(), mPath.length())
	//	<< "\n    BasePath: " << mBasePath
	//	<< "\n      Meshes: " << pScene->mNumMeshes
	//	<< "\n  Animations: " << pScene->mNumAnimations
	//	<< "\n   Materials: " << pScene->mNumMaterials
	//	<< "\n      Lights: " << pScene->mNumLights
	//	<< "\n     Cameras: " << pScene->mNumCameras
	//	<< "\n  Textures_1: " << pScene->mNumTextures
	//	<< "\n  Textures_2: " << nn << "\n" << std::endl;
}

void Model::ProcessHierarchy(const aiNode* pNode, const aiScene* pScene, const aiNode* pParentNode /* = nullptr */)
{
	// if pNode is parent, pParentNode = nullptr
	for (size_t i = 0; i < pNode->mNumMeshes; ++i) {
		size_t meshIndex = pNode->mMeshes[i];
		const aiMesh* pMesh = pScene->mMeshes[meshIndex];
	}

	for (size_t childIndex = 0; childIndex < pNode->mNumChildren; childIndex++) {
		const aiNode* pChildNode = pNode->mChildren[childIndex];
		ProcessHierarchy(pChildNode, pScene, pNode);
	}
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
}*/

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
		textures.push_back(std::move(g_resourceManager.loadTexture(fullPath)));
	}

	return std::move(textures);
}*/