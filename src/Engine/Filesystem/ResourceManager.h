
#ifndef ENGINE_FILESYSTEM_RESOURCEMANAGER_H
#define ENGINE_FILESYSTEM_RESOURCEMANAGER_H

#include "../Core/Model.h"
#include "File.h"
#include "AssimpIOStream.h"

class ResourceManager
{
public:
	static ResourceManager& GetInstance() {
		static ResourceManager instance;
		return instance;
	}

	bool Init();
	void Terminate();

	bool Mount(std::string directory /* dir/zip/... */);
	bool Exists(std::string directory);

	File_ptr OpenRead(std::string filename);
	File_ptr OpenWrite(std::string filename);

	Model_ptr LoadModel(std::string filename);

	bool IsInitialized() const { return mInitialized; }

private:
	bool mInitialized = false;

	std::unique_ptr<Assimp::Importer> mAssimpImporter;

	std::map<std::string, Model_ptr> mModelCache;
};

#endif // ENGINE_FILESYSTEM_RESOURCEMANAGER_H