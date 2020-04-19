
#include <framework/pch.h>

#include "model.h"
#include "framework/managers/resource_manager.h" // TODO ALAA: make sure this doesn't create any problems (circular dependency or smt)

using namespace framework::core;


template<typename T>
std::vector<Texture> Model<T>::LoadMeshTextures(aiMaterial* material, aiTextureType textureType, std::string folderPath) {
	std::vector<Texture> textures;
	for (unsigned int textureIndex = 0; textureIndex < material->GetTextureCount(textureType); textureIndex++) {
		/// Get the texture name
		aiString texName;
		material->GetTexture(textureType, textureIndex, &texName);
		
		/// Load the texture
		std::string fullPath = folderPath + texName.C_Str();
		textures.push_back(g_resourceManager.loadTexture(fullPath));
	}
	return std::move(textures);
}