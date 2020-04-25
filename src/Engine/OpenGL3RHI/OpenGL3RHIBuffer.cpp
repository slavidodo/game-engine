
#include "pch.h"

#include "OpenGL3RHIBuffer.h"

void OpenGL3RHIVertexBuffer::ReadData(size_t size, size_t offset, void* dest)
{
	//mBufferImpl->read(size, offset, dest);
}

void OpenGL3RHIVertexBuffer::WriteData(void* src, size_t size, size_t offset, bool discard)
{
	//mBufferImpl->write(src, size, offset, discard);
}

void OpenGL3RHIIndexBuffer::ReadData(size_t size, size_t offset, void* dest)
{
	//mBufferImpl->read(size, offset, dest);
}

void OpenGL3RHIIndexBuffer::WriteData(void* src, size_t size, size_t offset, bool discard)
{
	//mBufferImpl->write(src, size, offset, discard);
}