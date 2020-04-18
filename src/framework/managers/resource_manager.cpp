
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
	unsigned char* buffer = new unsigned char[length];

	if (file->read((void*)buffer, length) != length)
		return nullptr;

	return g_window.context()->makeTexture(buffer, length);
}




AssimpIOStream::AssimpIOStream(const char* fileName) {
	m_file = g_resourceManager.openRead(fileName);
}

AssimpIOStream::~AssimpIOStream() {
	m_file->close();
}

size_t AssimpIOStream::Read(void* buffer, size_t pSize, size_t pCount) {
	unsigned long long length = m_file->length();
	m_file->read(buffer, length);

	return length;
}

size_t AssimpIOStream::Write(const void* pvBuffer, size_t pSize, size_t pCount) {
	return m_file->write(pvBuffer, pCount);
}

size_t AssimpIOStream::FileSize() const {
	return m_file->length();
}

aiReturn AssimpIOStream::Seek(size_t pOffset, aiOrigin pOrigin) {
	if (pOrigin == aiOrigin_SET && m_file->seek(pOffset) != NULL)
		return aiReturn::aiReturn_SUCCESS;
	return aiReturn::aiReturn_FAILURE;
}

size_t AssimpIOStream::Tell() const {
	return m_file->tell();
}

void AssimpIOStream::Flush() {
	m_file->flush();
}




bool AssimpIOSystem::Exists(const char* fileName) const {
	return g_resourceManager.exists(fileName);
}

char AssimpIOSystem::getOsSeparator() const {
	return '/';
}

Assimp::IOStream* AssimpIOSystem::Open(const char* fileName, const char* pMode) {
	return new AssimpIOStream(fileName);
}

void AssimpIOSystem::Close(Assimp::IOStream* pFile) {
	pFile->~IOStream();
	delete pFile;
}