
#include "pch.h"

#include "File.h"

#include <physfs.h>

File::File(PHYSFS_File* handle)
	: mHandle(handle)
{
}

File::~File()
{
	Close();
}

bool File::Close()
{
	int success = 0;
	if (mHandle && PHYSFS_isInit()) {
		success = PHYSFS_close(mHandle);
		mHandle = nullptr;
	}

	return success != 0;
}

bool File::Flush()
{
	return PHYSFS_flush(mHandle) != 0;
}

bool File::Seek(uint64_t position)
{
	return PHYSFS_seek(mHandle, position) != 0;
}

uint64_t File::Tell()
{
	return PHYSFS_tell(mHandle);
}

uint64_t File::Length()
{
	return PHYSFS_fileLength(mHandle);
}

uint64_t File::Read(void* buf, size_t len, size_t count /* = 1*/)
{
	return PHYSFS_read(mHandle, buf, len, count);
}

uint64_t File::Write(const void* buf, size_t len, size_t count /* = 1*/)
{
	return PHYSFS_write(mHandle, buf, len, count);
}