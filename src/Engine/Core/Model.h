
#ifndef ENGINE_CORE_MODEL_H
#define ENGINE_CORE_MODEL_H

#include "Object.h"
#include "Camera.h"
#include "Transform.h"
#include "StaticMesh.h"
#include "Texture.h"

#include "../Scene/SceneNode.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include "../../Engine/RHI/RHICommandList.h"
#include "../Core/StaticMesh.h"
#include "Engine\StaticMeshGenerator.h"

class Model;
typedef std::shared_ptr<Model> Model_ptr;

// only loads meshes from model, for loading camera and lights use gltf scene loader 
class Model : public std::enable_shared_from_this<Model>
{
public:
	Model(std::string path, const aiScene* pScene);
	~Model();
	void RenderSceneElements(RHICommandList& RHICmdList);
	StaticMesh_ptr GetFirstMesh();
private:
	void Load(const aiScene* pScene);
	void ProcessHierarchy(const aiNode* pNode, const aiScene* pScene, const aiNode* pParentNode = nullptr);

	//Camera_ptr MakeCamera(const aiCamera* pCamera);
	// Light_ptr MakeLight(const aiLight* pLight);

	 //VertexFormat GetVertexFormat(const aiMesh* pMesh);
	 //void* GetVertexData(const aiMesh* pMesh);
	 //void* GetIndexData(const aiMesh* pMesh);

	//std::vector<Texture_ptr> LoadTextures(const aiMaterial* pMaterial, aiTextureType textureType);
	//Texture_ptr LoadMaterialTexture(aiString texturePath);

	SceneNode_ptr mRootNode;
	std::vector< StaticMesh_ptr > mStaticMeshList;
	bool mModelReady = false;

	std::string mPath;
	std::string mParentPath;
	std::string mFilename;

	SceneNode_ptr CreateSceneNode(const aiNode* pNode);

	void CreateStaticMeshWithDefaultFilter(StaticMesh_ptr mesh, StaticMeshCreateInfo& info);
	std::vector<StaticMeshVertex> GetMeshVertices(const aiMesh* pMesh);
	std::vector<int32_t> GetMeshIndices(const aiMesh* pMesh);
	//std::vector<Texture_ptr> LoadMeshTextures(aiMaterial* pMaterial, aiTextureType textureType, std::string folderPath);
};

#endif // ENGINE_CORE_MODEL_H