
#include "pch.h"

#include "AssimpIOStream.h"

#include "ResourceManager.h"

AssimpIOStream::AssimpIOStream(std::string filename)
{
	mFile = ResourceManager::GetInstance().OpenRead(filename);
}

AssimpIOStream::~AssimpIOStream()
{
	mFile->Close();
}

size_t AssimpIOStream::Read(void* buffer, size_t pSize, size_t pCount) {
	return (size_t)mFile->Read(buffer, pCount, pSize);
}

size_t AssimpIOStream::Write(const void* pvBuffer, size_t pSize, size_t pCount) {
	return (size_t)mFile->Write(pvBuffer, pCount, pSize);
}

size_t AssimpIOStream::FileSize() const {
	return (size_t)mFile->Length();
}

aiReturn AssimpIOStream::Seek(size_t pOffset, aiOrigin pOrigin) {
	if (pOrigin == aiOrigin_SET && mFile->Seek(pOffset))
		return aiReturn::aiReturn_SUCCESS;
	return aiReturn::aiReturn_FAILURE;
}

size_t AssimpIOStream::Tell() const {
	return (size_t)mFile->Tell();
}

void AssimpIOStream::Flush() {
	mFile->Flush();
}

////////////////////////////////////////

bool AssimpIOSystem::Exists(const char* fileName) const {
	return ResourceManager::GetInstance().Exists(fileName);
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