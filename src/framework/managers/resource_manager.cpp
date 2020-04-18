
#include <framework/pch.h>

#include "resource_manager.h"

#include <framework/platform/window.h>

using namespace framework::managers;
using namespace framework;

ResourceManager g_resourceManager;

bool ResourceManager::init()
{
	if (m_initialized)
		return true;

	m_initialized = PHYSFS_init(nullptr) == 0;
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
	unsigned char* buffer = new unsigned char[length];

	if (file->read((void*)buffer, length) != length)
		return nullptr;

	return g_window.context()->makeTexture(buffer);
}

core::Model_ptr ResourceManager::loadModel(std::string filename)
{
	filesystem::File_ptr file = openRead(filename);
	if (!file)
		return nullptr;

	return loadModel(std::move(file));
}

core::Model_ptr ResourceManager::loadModel(filesystem::File_ptr file)
{
	// TODO; REHAN
	return nullptr;
}