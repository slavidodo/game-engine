
#include "pch.h"

#include "assimp_iostream.h"

#include <framework/managers/resource_manager.h>

using namespace framework::filesystem;

AssimpIOStream::AssimpIOStream(std::string filename)
{
	m_file = g_resourceManager.openRead(filename);
}

AssimpIOStream::~AssimpIOStream()
{
	m_file->close();
}

size_t AssimpIOStream::Read(void* buffer, size_t pSize, size_t pCount) {
	return (size_t)m_file->read(buffer, pCount, pSize);
}

size_t AssimpIOStream::Write(const void* pvBuffer, size_t pSize, size_t pCount) {
	return (size_t)m_file->write(pvBuffer, pCount, pSize);
}

size_t AssimpIOStream::FileSize() const {
	return (size_t)m_file->length();
}

aiReturn AssimpIOStream::Seek(size_t pOffset, aiOrigin pOrigin) {
	if (pOrigin == aiOrigin_SET && m_file->seek(pOffset) != NULL)
		return aiReturn::aiReturn_SUCCESS;
	return aiReturn::aiReturn_FAILURE;
}

size_t AssimpIOStream::Tell() const {
	return (size_t)m_file->tell();
}

void AssimpIOStream::Flush() {
	m_file->flush();
}

////////////////////////////////////////

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