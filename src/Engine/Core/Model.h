
#ifndef ENGINE_CORE_MODEL_H
#define ENGINE_CORE_MODEL_H

#include "Object.h"
#include "Camera.h"
#include "Transform.h"
#include "Mesh.h"
#include "Texture.h"

#include "../Scene/SceneNode.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

class Model;
typedef std::shared_ptr<Model> Model_ptr;

class Model : public std::enable_shared_from_this<Model>
{
public:
	Model(std::string path, const aiScene* pScene);
	~Model();

private:
	void Load(const aiScene* pScene);
	void ProcessHierarchy(const aiNode* pNode, const aiScene* pScene, const aiNode* pParentNode = nullptr);

	//Camera_ptr MakeCamera(const aiCamera* pCamera);
	// Light_ptr MakeLight(const aiLight* pLight);

	// VertexFormat GetVertexFormat(const aiMesh* pMesh);
	// void* GetVertexData(const aiMesh* pMesh);
	// void* GetIndexData(const aiMesh* pMesh);

	//std::vector<Texture_ptr> LoadTextures(const aiMaterial* pMaterial, aiTextureType textureType);
	//Texture_ptr LoadMaterialTexture(aiString texturePath);

	SceneNode_ptr mRootNode;

	std::string mPath;
	std::string mParentPath;
	std::string mFilename;

	//std::vector<Vertex1P1D1S1M1UV> getMeshVertices(aiMesh* pMesh);
	std::vector<int32_t> GetMeshIndices(aiMesh* pMesh);
	//std::vector<Texture_ptr> LoadMeshTextures(aiMaterial* pMaterial, aiTextureType textureType, std::string folderPath);
};

#endif // ENGINE_CORE_MODEL_H