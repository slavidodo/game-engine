
#ifndef FRAMEWORK_CORE_MODEL_H
#define FRAMEWORK_CORE_MODEL_H

#include <framework/core/mesh.h>
#include <framework/core/texture.h>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

namespace framework {
namespace core {

class Model;
typedef std::shared_ptr<Model> Model_ptr;

class Model : public std::enable_shared_from_this<Model>
{
public:
	Model(std::string filePath, const aiScene* sceneObject) {
		std::string folderPath = filePath.substr(0, filePath.find_last_of('/') + 1);
		processNode(sceneObject->mRootNode, sceneObject, folderPath);
	}

private:
	std::vector<core::Mesh_ptr> m_meshes;

	void processNode(const aiNode* node, const aiScene* sceneObject, std::string folderPath);

	//std::vector<Vertex1P1D1S1M1UV> getMeshVertices(aiMesh* pMesh);
	std::vector<int> getMeshIndices(aiMesh* pMesh);
	std::vector<Texture_ptr> loadMeshTextures(aiMaterial* pMaterial, aiTextureType textureType, std::string folderPath);
};

} // ns core
} // ns framework

#endif