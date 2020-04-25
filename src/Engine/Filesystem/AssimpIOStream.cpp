
#include "pch.h"

#include "AssimpIOStream.h"

#include "ResourceManager.h"

AssimpIOStream::AssimpIOStream(std::string filename)
{
	mFile = ResourceManager::GetInstance().openRead(filename);
}

AssimpIOStream::~AssimpIOStream()
{
	mFile->close();
}

size_t AssimpIOStream::Read(void* buffer, size_t pSize, size_t pCount) {
	return (size_t)mFile->read(buffer, pCount, pSize);
}

size_t AssimpIOStream::Write(const void* pvBuffer, size_t pSize, size_t pCount) {
	return (size_t)mFile->write(pvBuffer, pCount, pSize);
}

size_t AssimpIOStream::FileSize() const {
	return (size_t)mFile->length();
}

aiReturn AssimpIOStream::Seek(size_t pOffset, aiOrigin pOrigin) {
	if (pOrigin == aiOrigin_SET && mFile->seek(pOffset) != NULL)
		return aiReturn::aiReturn_SUCCESS;
	return aiReturn::aiReturn_FAILURE;
}

size_t AssimpIOStream::Tell() const {
	return (size_t)mFile->tell();
}

void AssimpIOStream::Flush() {
	mFile->flush();
}

////////////////////////////////////////

bool AssimpIOSystem::Exists(const char* fileName) const {
	return ResourceManager::GetInstance().exists(fileName);
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