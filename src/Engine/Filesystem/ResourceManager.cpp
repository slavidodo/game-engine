
#include "pch.h"

#include "ResourceManager.h"

#include <physfs.h>

bool ResourceManager::Init()
{
	if (mInitialized)
		return true;

	mInitialized = PHYSFS_init(nullptr) != 0;

	if (mInitialized) {
		// initialize assimp importers
		mAssimpImporter.reset(new Assimp::Importer);
		mAssimpImporter->SetIOHandler(new AssimpIOSystem);
	}

	return mInitialized;
}

void ResourceManager::Terminate()
{
	PHYSFS_deinit();
}

bool ResourceManager::Mount(std::string directory)
{
	return PHYSFS_mount(directory.c_str(), nullptr, 0) != 0;
}

bool ResourceManager::Exists(std::string directory) {
	return PHYSFS_exists(directory.c_str());
}

File_ptr ResourceManager::OpenRead(std::string filename)
{
	PHYSFS_File* file = PHYSFS_openRead(filename.c_str());
	if (!file)
		return nullptr;

	return std::move(std::make_shared<File>(file));
}

File_ptr ResourceManager::OpenWrite(std::string filename)
{
	PHYSFS_File* file = PHYSFS_openWrite(filename.c_str());
	if (!file)
		return nullptr;

	return std::move(std::make_shared<File>(file));
}

Model_ptr ResourceManager::LoadModel(std::string filename)
{
	// todo; lock and do this on another thread
	// the returned model should be owned again by main thread
	// once the data is ready

	// todo; let user specify whether to modify the model in order
	// to make it suitable for a specific situation

	// todo: let rhi add its own flags
	// D3D prefers left-handed coordinate space
	uint32_t flags = aiProcess_CalcTangentSpace // calculate tangent if normals exist
		| aiProcess_Triangulate // triangulate?
		| aiProcess_JoinIdenticalVertices // get rid of duplicated vertices
		| aiProcess_SortByPType // split primitives
		| aiProcess_OptimizeGraph; // get rid of unnecessary nodes

	const aiScene* scene = mAssimpImporter->ReadFile(filename, flags);
	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		return nullptr;
	}

	auto model = std::make_shared<Model>(filename, scene);
	mModelCache.emplace(filename, model); // are we gonna load everything into memory? no?
	mAssimpImporter->FreeScene();
	return model;
}