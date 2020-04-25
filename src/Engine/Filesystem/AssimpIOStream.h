
#ifndef ENGINE_FILESYSTEM_ASSIMPIOSTREAM_H
#define ENGINE_FILESYSTEM_ASSIMPIOSTREAM_H

#include "file.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/ZipArchiveIOSystem.h>

class AssimpIOStream;
typedef std::shared_ptr<AssimpIOStream> AssimpIOStream_ptr;

class AssimpIOSystem : public Assimp::IOSystem
{
public:
	AssimpIOSystem() = default;
	~AssimpIOSystem() = default;

	bool Exists(const char* fileName) const override final;
	char getOsSeparator() const override final;

	Assimp::IOStream* Open(const char* fileName, const char* pMode) override final;
	void Close(Assimp::IOStream* pFile) override final;
};

class AssimpIOStream : public Assimp::IOStream, public std::enable_shared_from_this<AssimpIOStream>
{
public:
	AssimpIOStream(std::string filename);
	~AssimpIOStream();

	size_t Read(void* buffer, size_t pSize, size_t pCount) override final;
	size_t Write(const void* pvBuffer, size_t pSize, size_t pCount) override final;

	size_t FileSize() const override final;

	aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override final;
	size_t Tell() const override final;
	void Flush() override final;

private:
	File_ptr mFile;
};

#endif // ENGINE_FILESYSTEM_ASSIMPIOSTREAM_H