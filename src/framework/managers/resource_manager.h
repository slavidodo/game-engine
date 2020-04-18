
#ifndef FRAMEWORK_MANAGERS_RESOURCEMANAGER_H
#define FRAMEWORK_MANAGERS_RESOURCEMANAGER_H

#include <framework/core/texture.h>
#include <framework/core/model.h>
#include <framework/filesystem/file.h>

#include <physfs.h>

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/ZipArchiveIOSystem.h"

namespace framework {
namespace managers {

class AssimpIOStream : public Assimp::IOStream {
private:
	filesystem::File_ptr m_file;
	friend class AssimpIOSystem;

protected:
	AssimpIOStream(const char* fileName);

public:
	~AssimpIOStream();

	size_t Read(void* buffer, size_t pSize, size_t pCount);
	size_t Write(const void* pvBuffer, size_t pSize, size_t pCount);

	size_t FileSize() const;

	aiReturn Seek(size_t pOffset, aiOrigin pOrigin);
	size_t Tell() const;
	void Flush();
};
class AssimpIOSystem : public Assimp::IOSystem {
public:
	AssimpIOSystem() = default;
	~AssimpIOSystem() = default;

	bool Exists(const char* fileName) const;
	char getOsSeparator() const;

	Assimp::IOStream* Open(const char* fileName, const char* pMode);

	void Close(Assimp::IOStream* pFile);
};


class ResourceManager
{
public:

	bool init();
	void terminate();

	bool mount(std::string directory /* dir/zip/... */);
	bool exists(std::string directory);

	filesystem::File_ptr openRead(std::string filename);
	filesystem::File_ptr openWrite(std::string filename);

	core::Texture_ptr loadTexture(std::string filename);
	core::Texture_ptr loadTexture(filesystem::File_ptr file);

	template <typename T>
	core::BaseModel_ptr loadModel(std::string filename) {
		Assimp::Importer importer;
		AssimpIOSystem* ioSystem = new AssimpIOSystem();
		importer.SetIOHandler(ioSystem);

		const aiScene* sceneObject = importer.ReadFile(
			filename,
			aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
		); // TODO: give the user the choice of flags?

		if (sceneObject == nullptr || sceneObject->mFlags & AI_SCENE_FLAGS_INCOMPLETE || sceneObject->mRootNode == NULL)
			return nullptr;

		return loadModel<T>(filename, sceneObject);
	}
	template <typename T>
	core::BaseModel_ptr loadModel(std::string filename, const aiScene* sceneObject) {
		return std::move(std::shared_ptr<core::Model>(filename, sceneObject));
	}

	OBJECT_GETACCESSOR(bool, bool, initialized);

private:
	bool m_initialized = false;

	std::map<std::string, core::Texture_ptr> m_textureCache;
	std::map<std::string, core::BaseModel_ptr> m_modelCache;
};

} // ns managers
} // ns framework

extern framework::managers::ResourceManager g_resourceManager;

#endif