
#include <framework/pch.h>

#include "file.h"

#include <physfs.h>

using namespace framework::filesystem;

File::File(PHYSFS_File* handle)
	: m_handle(handle)
{
}

File::~File()
{
	close();
}

bool File::close()
{
	int success = 0;
	if (m_handle && PHYSFS_isInit()) {
		success = PHYSFS_close(m_handle);
		m_handle = nullptr;
	}

	return success != 0;
}

bool File::flush()
{
	return PHYSFS_flush(m_handle) != 0;
}

bool File::seek(uint64_t position)
{
	return PHYSFS_seek(m_handle, position) != 0;
}

uint64_t File::tell()
{
	return PHYSFS_tell(m_handle);
}

uint64_t File::length()
{
	return PHYSFS_fileLength(m_handle);
}

uint64_t File::read(void* buf, uint32_t len, uint32_t count /* = 1*/)
{
	return PHYSFS_read(m_handle, buf, len, count);
}

uint64_t File::write(const void* buf, uint32_t len, uint32_t count /* = 1*/)
{
	return PHYSFS_write(m_handle, buf, len, count);
}