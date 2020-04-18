
#ifndef FRAMEWORK_FILESYSTEM_FILE_H
#define FRAMEWORK_FILESYSTEM_FILE_H

struct PHYSFS_File;

namespace framework {
namespace filesystem {

class File;
typedef std::shared_ptr<File> File_ptr;

class File : std::enable_shared_from_this<File>
{
public:
	explicit File(PHYSFS_File* handle);
	~File();

	bool close();
	bool flush();

	bool seek(uint64_t position);
	uint64_t tell();
	uint64_t length();
	uint64_t read(void* buf, uint32_t len);
	uint64_t write(const void* buf, uint32_t len);

private:
	PHYSFS_File* m_handle;
};

} // filesystem
} // framework

#endif