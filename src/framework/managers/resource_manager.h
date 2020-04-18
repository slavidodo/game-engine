
#ifndef FRAMEWORK_MANAGERS_RESOURCEMANAGER_H
#define FRAMEWORK_MANAGERS_RESOURCEMANAGER_H

#include <framework/core/texture.h>
#include <framework/core/model.h>
#include <framework/filesystem/file.h>

#include <physfs.h>

namespace framework {
namespace managers {

class ResourceManager
{
public:

	bool init();
	void terminate();

	bool mount(std::string directory /* dir/zip/... */);

	filesystem::File_ptr openRead(std::string filename);
	filesystem::File_ptr openWrite(std::string filename);

	core::Texture_ptr loadTexture(std::string filename);
	core::Texture_ptr loadTexture(filesystem::File_ptr file);

	core::Model_ptr loadModel(std::string filename);
	core::Model_ptr loadModel(filesystem::File_ptr file);

	OBJECT_GETACCESSOR(bool, bool, initialized);

private:
	bool m_initialized = false;

	std::map<std::string, core::Texture_ptr> m_textureCache;
	std::map<std::string, core::Model_ptr> m_modelCache;
};

} // ns managers
} // ns framework

extern framework::managers::ResourceManager g_resourceManager;

#endif