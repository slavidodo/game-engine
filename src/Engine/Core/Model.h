
#ifndef ENGINE_CORE_MODEL_H
#define ENGINE_CORE_MODEL_H

#include "Mesh.h"
#include "Image.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

class Model;
typedef std::shared_ptr<Model> Model_ptr;

class Model : public std::enable_shared_from_this<Model>
{
public:
	Model(std::string filePath, const aiScene* sceneObject) {
		mFolderPath = filePath.substr(0, filePath.find_last_of('/') + 1);
		ProcessNode(sceneObject->mRootNode, sceneObject);
	}

private:
	std::vector<Mesh_ptr> mMeshes;
	std::string mFolderPath;

	void ProcessNode(const aiNode* node, const aiScene* sceneObject);

	//std::vector<Vertex1P1D1S1M1UV> getMeshVertices(aiMesh* pMesh);
	std::vector<int32_t> GetMeshIndices(aiMesh* pMesh);
	std::vector<RHITexture_ptr> LoadMeshTextures(aiMaterial* pMaterial, aiTextureType textureType, std::string folderPath);
};

#endif // ENGINE_CORE_MODEL_H