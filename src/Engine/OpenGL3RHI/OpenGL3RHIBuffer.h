
#ifndef ENGINE_OPENGL3RHI_OPENGL3RHIBUFER_H
#define ENGINE_OPENGL3RHI_OPENGL3RHIBUFER_H

#include "../RHI/RHIBuffer.h"
#include "OpenGl3RHIBufferImpl.h"

class OpenGL3RHIVertexBuffer : public RHIVertexBuffer
{
public:
	OpenGL3RHIVertexBuffer(size_t verticesCount, size_t vertexSize, RHIHardwareBufferUsage usage)
		: RHIVertexBuffer(verticesCount, vertexSize, usage) {
		//m_bufferImpl.reset(new OpenGLRHIHwBufferImpl(GL_ARRAY_BUFFER, usage, m_size));
	}

	void ReadData(size_t size, size_t offset, void* dest) override final;
	void WriteData(void* src, size_t size, size_t offset, bool discard) override final;

private:
	//std::unique_ptr<OpenGLRHIHwBufferImpl> m_bufferImpl;
};

class OpenGL3RHIIndexBuffer : public RHIIndexBuffer
{
public:
	OpenGL3RHIIndexBuffer(size_t indicesCount, RHIHardwareBufferUsage usage, RHIIndexBufferType indexType = RHIIndexBufferType::IBT_32)
		: RHIIndexBuffer(indicesCount, usage, indexType) {
		//m_bufferImpl.reset(new OpenGLRHIHwBufferImpl(GL_ELEMENT_ARRAY_BUFFER, usage, m_size));
	}

	constexpr size_t getSize() {
		return sizeof(size_t);
	}

	void ReadData(size_t size, size_t offset, void* dest) override final;
	void WriteData(void* data, size_t size, size_t offset, bool discard) override final;
private:
	//std::unique_ptr<OpenGLRHIHwBufferImpl> m_bufferImpl;
};

#endif // ENGINE_OPENGL3RHI_OPENGL3RHIBUFER_H
