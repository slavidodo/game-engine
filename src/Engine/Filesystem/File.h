
#ifndef ENGINE_FILESYSTEM_FILE_H
#define ENGINE_FILESYSTEM_FILE_H

struct PHYSFS_File;

class File;
typedef std::shared_ptr<File> File_ptr;

class File : public std::enable_shared_from_this<File>
{
public:
	explicit File(PHYSFS_File* handle);
	~File();

	bool Close();
	bool Flush();

	bool Seek(uint64_t position);
	uint64_t Tell();
	uint64_t Length();
	uint64_t Read(void* buf, uint32_t len, uint32_t count = 1);
	uint64_t Write(const void* buf, uint32_t len, uint32_t count = 1);

private:
	PHYSFS_File* mHandle;
};

#endif // ENGINE_FILESYSTEM_FILE_H