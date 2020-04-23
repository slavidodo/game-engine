
#include "pch.h"

#include "resource_manager.h"

#include <framework/platform/window.h>
#include <physfs.h>

using namespace framework::managers;
using namespace framework;

ResourceManager g_resourceManager;

bool ResourceManager::init()
{
	if (m_initialized)
		return true;

	m_initialized = PHYSFS_init(nullptr) != 0;
	return m_initialized;
}

void ResourceManager::terminate()
{
	PHYSFS_deinit();
}

bool ResourceManager::mount(std::string directory)
{
	return PHYSFS_mount(directory.c_str(), nullptr, 0) != 0;
}

bool ResourceManager::exists(std::string directory) {
	return PHYSFS_exists(directory.c_str());
}

filesystem::File_ptr ResourceManager::openRead(std::string filename)
{
	PHYSFS_File* file = PHYSFS_openRead(filename.c_str());
	if (!file)
		return nullptr;

	return std::move(std::make_shared<framework::filesystem::File>(file));
}

filesystem::File_ptr ResourceManager::openWrite(std::string filename)
{
	PHYSFS_File* file = PHYSFS_openWrite(filename.c_str());
	if (!file)
		return nullptr;

	return std::move(std::make_shared<framework::filesystem::File>(file));
}

core::Texture_ptr ResourceManager::loadTexture(std::string filename)
{
	filesystem::File_ptr file = openRead(filename);
	if (!file)
		return nullptr;

	return loadTexture(std::move(file));
}

core::Texture_ptr ResourceManager::loadTexture(filesystem::File_ptr file)
{
	size_t length = (size_t)file->length();
	uint8_t* buffer = new uint8_t[length];

	if (file->read((void*)buffer, length) != length)
		return nullptr;

	return g_window.context()->makeTexture(buffer, length);
}

core::Model_ptr ResourceManager::loadModel(std::string filename)
{
	Assimp::Importer importer;
	importer.SetIOHandler(new filesystem::AssimpIOSystem);

	const aiScene* scene = importer.ReadFile(
		filename,
		aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
	); // TODO: give the user the choice of flags?

	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		return nullptr;
	}

	auto model = std::make_shared<core::Model>(filename, scene);
	return model;
}