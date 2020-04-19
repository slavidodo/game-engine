
#ifndef FRAMEWORK_CORE_MODEL_H
#define FRAMEWORK_CORE_MODEL_H

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include "framework/core/mesh.h"
#include "framework/core/texture.h"


namespace framework {
namespace core {

// TODO ALAA: no idea if we need a base model class, I just tried to stay close to what you did with the mesh class
class BaseModel;
typedef std::shared_ptr<BaseModel> BaseModel_ptr;

class BaseModel : public std::enable_shared_from_this<BaseModel> {
protected:
	virtual void ProcessNode(const aiNode* node, const aiScene* sceneObject, std::string folderPath) = 0;
	virtual std::vector<int> GetMeshIndices(aiMesh* mesh) = 0;
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

			/// Create the mesh object (vertices & indices)
			int globalMeshIndex = node->mMeshes[meshIndex]; // index in the (scene object's) meshes array
			aiMesh* pMesh = sceneObject->mMeshes[globalMeshIndex]; // mesh in the (scene object's) meshes array
			Mesh<VertexType> mesh = CreateMesh(pMesh);
			


			/// Get the mesh's material from the (scene object's) materials array
			int materialIndex = pMesh->mMaterialIndex;
			if (materialIndex >= 0) {
				aiMaterial* material = sceneObject->mMaterials[materialIndex];

				/// Get the material's textures
				std::vector<Texture> ambientTextures  = LoadMeshTextures(material, aiTextureType_AMBIENT, folderPath);
				std::vector<Texture> diffuseTextures  = LoadMeshTextures(material, aiTextureType_DIFFUSE, folderPath);
				std::vector<Texture> specularTextures = LoadMeshTextures(material, aiTextureType_SPECULAR, folderPath);
				std::vector<Texture> normalTextures   = LoadMeshTextures(material, aiTextureType_NORMALS, folderPath);
				std::vector<Texture> heightTextures   = LoadMeshTextures(material, aiTextureType_HEIGHT, folderPath);

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

	BaseMesh_ptr CreateMesh(aiMesh* mesh) {
		std::vector<VertexType> vertices = GetMeshVertices<VertexType>(mesh);
		std::vector<int> indices = GetMeshIndices(mesh);
		return std::move(std::shared_ptr<BaseMesh_ptr>(std::move(vertices), std::move(indices)));
		// TODO ALAA: did I create a mesh object correctly ??
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

	std::vector<Texture> LoadMeshTextures(aiMaterial* material, aiTextureType textureType, std::string folderPath);
};

} // ns core
} // ns framework

#endif