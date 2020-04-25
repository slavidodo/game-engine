
#include "pch.h"

#include "Model.h"

#include "../Filesystem/ResourceManager.h"

void Model::processNode(const aiNode* node, const aiScene* sceneObject, std::string folderPath)
{
	for (size_t meshIndex = 0; meshIndex < node->mNumMeshes; meshIndex++) {
		uint32_t globalMeshIndex = node->mMeshes[meshIndex];
		aiMesh* pMesh = sceneObject->mMeshes[globalMeshIndex];

		// obtain the within its descriptor to ensure how valid it's
		//auto mesh = g_window.context()->makeMesh<Vertex1P1D1S1M1UV>(getMeshVertices(pMesh), getMeshIndices(pMesh));

		if (pMesh->mMaterialIndex >= 0) {
			aiMaterial* pMaterial = sceneObject->mMaterials[pMesh->mMaterialIndex];

			//std::vector<Texture_ptr> ambientTextures = loadMeshTextures(pMaterial, aiTextureType_AMBIENT, folderPath);
			//std::vector<Texture_ptr> diffuseTextures = loadMeshTextures(pMaterial, aiTextureType_DIFFUSE, folderPath);
			//std::vector<Texture_ptr> specularTextures = loadMeshTextures(pMaterial, aiTextureType_SPECULAR, folderPath);
			//std::vector<Texture_ptr> normalTextures = loadMeshTextures(pMaterial, aiTextureType_NORMALS, folderPath);
			//std::vector<Texture_ptr> heightTextures = loadMeshTextures(pMaterial, aiTextureType_HEIGHT, folderPath);

			// todo link to material
		}

		//mMeshes.push_back(std::move(mesh));
	}

	// process children nodes
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; childIndex++) {
		aiNode* pChildNode = node->mChildren[childIndex];
		processNode(pChildNode, sceneObject, folderPath);
	}
}

/*std::vector<Vertex1P1D1S1M1UV> Model::getMeshVertices(aiMesh* pMesh)
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

std::vector<int> Model::getMeshIndices(aiMesh* mesh)
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