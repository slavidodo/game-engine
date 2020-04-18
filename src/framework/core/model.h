
#ifndef FRAMEWORK_CORE_MODEL_H
#define FRAMEWORK_CORE_MODEL_H

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include "framework/core/mesh.h"
#include "framework/core/texture.h"

//framework::managers::ResourceManager g_resourceManager;

namespace framework {
namespace core {

class BaseModel;
typedef std::shared_ptr<BaseModel> BaseModel_ptr;

class BaseModel : public std::enable_shared_from_this<BaseModel> {
protected:
	std::vector<Texture_ptr> m_loadedTextures;

	virtual void ProcessNode(const aiNode* node, const aiScene* sceneObject, std::string folderPath) = 0;
	virtual std::vector<int> GetMeshIndices(aiMesh* mesh) = 0;
	virtual std::vector<Texture> LoadMeshTextures(aiMaterial* material, aiTextureType textureType) = 0;
};

template<typename T>
class Model : public BaseModel
{
public:
	typedef T VertexType;
	
	Model(std::string filePath, aiScene* sceneObject) {
		std::string folderPath = filePath.substr(0, filePath.find_last_of('/') + 1);
		ProcessNode(sceneObject->mRootNode, sceneObject, folderPath);
	}

private:
	std::vector<core::Mesh<VertexType>> m_meshes;

	void ProcessNode(const aiNode* node, const aiScene* sceneObject, std::string folderPath) override final {
		/// Create/Push mesh objects to the "m_meshes" vector
		for (unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; meshIndex++) { // index in the (node's) meshes array

			/// Create the mesh object
			int globalMeshIndex = node->mMeshes[meshIndex]; // index in the (scene object's) meshes array
			aiMesh* pMesh = sceneObject->mMeshes[globalMeshIndex]; // mesh in the (scene object's) meshes array

			// TODO ALAA: did I create a mesh object correctly ??
			std::vector<VertexType> vertices = GetMeshVertices<VertexType>(pMesh);
			std::vector<int> indices = GetMeshIndices(pMesh);
			Mesh<VertexType> mesh(std::move(vertices), std::move(indices));


			/// Get the mesh's material from the (scene object's) materials array
			int materialIndex = pMesh->mMaterialIndex;
			if (materialIndex >= 0) {
				aiMaterial* material = sceneObject->mMaterials[materialIndex];

				/// Get the material's textures
				std::vector<Texture> ambientTex = LoadMeshTextures(material, aiTextureType_AMBIENT, folderPath);
				std::vector<Texture> diffuseTex = LoadMeshTextures(material, aiTextureType_DIFFUSE, folderPath);
				std::vector<Texture> specularTex = LoadMeshTextures(material, aiTextureType_SPECULAR, folderPath);
				std::vector<Texture> normalTex = LoadMeshTextures(material, aiTextureType_NORMALS, folderPath);
				std::vector<Texture> heightTex = LoadMeshTextures(material, aiTextureType_HEIGHT, folderPath);

				// TODO ALAA: link the textures to the mesh somehow
			}

			m_meshes.push_back(std::move(mesh));
		}

		/// Process the children nodes (recursive calls)
		for (unsigned int childIndex = 0; childIndex < node->mNumChildren; childIndex++) {
			aiNode* pChildNode = node->mChildren[childIndex];
			ProcessNode(pChildNode, sceneObject);
		}
	}

	std::vector<VertexType> GetMeshVertices(aiMesh* mesh) {
		std::vector<VertexType> vertices;
		for (unsigned int vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
			VertexType vertex;
			
			/// Get position
			{
				vertex.position.x = mesh->mVertices[vertexIndex].x;
				vertex.position.y = mesh->mVertices[vertexIndex].y;
				vertex.position.z = mesh->mVertices[vertexIndex].z;
			}

			// TODO ALAA: needs handling if the vertex has no normals or tex coords
			/// Get normals
			{
				/*vertex.normal.x = mesh->mNormals[vertexIndex].x;
				vertex.normal.y = mesh->mNormals[vertexIndex].y;
				vertex.normal.z = mesh->mNormals[vertexIndex].z;*/
			}
			/// Get texture coordinates
			{
				/*if (mesh->mTextureCoords[0] == NULL) {
					vertex.textureCoordinates = glm::vec2(0.0f);
				} else {
					vertex.textureCoordinates.x = mesh->mTextureCoords[0][vertexIndex].x;
					vertex.textureCoordinates.y = mesh->mTextureCoords[0][vertexIndex].y;
				}*/
			}

			vertices.push_back(vertex);
		}
		return std::move(vertices);
	}
	
	std::vector<int> GetMeshIndices(aiMesh* mesh) override final {
		std::vector<int> indices;
		for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
			aiFace face = mesh->mFaces[faceIndex];
			for (unsigned int indexIndex = 0; indexIndex < face.mNumIndices; indexIndex++)
				indices.push_back(face.mIndices[indexIndex]);
		}
		return std::move(indices);
	}

	std::vector<Texture> LoadMeshTextures(aiMaterial* material, aiTextureType textureType, std::string folderPath) override final {
		std::vector<Texture> textures;
		for (unsigned int textureIndex = 0; textureIndex < material->GetTextureCount(textureType); textureIndex++) {
			/// Get the texture name
			aiString texName;
			material->GetTexture(textureType, textureIndex, &texName);
			
			std::string fullPath = folderPath + texName.C_Str();
			//framework::managers::g_resourceManager.loadTexture(fullPath);
			// TODO ALAA: solve this problem

			/// Push the texture into our vector
			{
				//bool bAlreadyLoaded = false;
				//
				///// Check if this texture was loaded before
				//for (Texture& loadedTexture : m_loadedTextures) {
				//	if (std::strcmp(loadedTexture.GetPath().c_str(), path.C_Str()) == NULL) { // match
				//		bAlreadyLoaded = true;
				//		textures.push_back(loadedTexture);
				//		break;
				//	}
				//}


				///// If not, load the texture first
				//if (!bAlreadyLoaded) {
				//	const std::string fullPath = folderPath + path.C_Str();

				//	/// Create a new texture and push it into the model's loaded textures
				//	m_loadedTextures.push_back(g_resourceManager->loadTexture(fullPath));

				//	/// Push the texture into the mesh's textures vector
				//	textures.push_back(m_loadedTextures.back());
				//}
			}
		}
		return std::move(textures);
	}
};

} // ns core
} // ns framework

#endif