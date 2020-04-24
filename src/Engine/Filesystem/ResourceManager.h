
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

	bool mount(std::string directory /* dir/zip/... */);
	bool exists(std::string directory);

	File_ptr openRead(std::string filename);
	File_ptr openWrite(std::string filename);

	Model_ptr loadModel(std::string filename);

	OBJECT_GETACCESSOR(bool, bool, initialized);

private:
	bool m_initialized = false;

	std::map<std::string, Model_ptr> m_modelCache;
};

#endif // ENGINE_FILESYSTEM_RESOURCEMANAGER_H