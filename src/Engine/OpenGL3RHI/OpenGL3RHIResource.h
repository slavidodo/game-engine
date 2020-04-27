
#ifndef ENGINE_OPENGL3RHI_OPENGL3RHIRESOURCE_H
#define ENGINE_OPENGL3RHI_OPENGL3RHIRESOURCE_H

#include "../RHI/RHIResource.h"

class OpenGl3RHIBufferImpl
{
public:
	OpenGl3RHIBufferImpl(GLenum target, RHIHardwareBufferUsage usage, size_t size)
		: mTarget(target), mSize(size)
	{
		mGlUsage = GetGLUsageForHardwareUsage(usage);
		mBuffer = malloc(mSize);
		glGenBuffers(1, &mBufferId);
	}

	~OpenGl3RHIBufferImpl() {
		delete[] mBuffer;
	}

	OpenGl3RHIBufferImpl(OpenGl3RHIBufferImpl&) = delete;
	OpenGl3RHIBufferImpl& operator= (const OpenGl3RHIBufferImpl&) = delete;

	inline void* GetBuffer(void) {
		return mBuffer;
	}

	static GLenum GetGLUsageForHardwareUsage(RHIHardwareBufferUsage hbUsage)
	{
		if (hbUsage & RHIHardwareBufferUsage::HWBU_DISCARD) {
			return GL_STREAM_DRAW;
		} else if (hbUsage & RHIHardwareBufferUsage::HWBU_STATIC) {
			return GL_STATIC_DRAW;
		} else {
			return GL_DYNAMIC_DRAW;
		}
	}

private:
	void* mBuffer;

	size_t mSize;

	GLenum mGlUsage;
	GLenum mTarget;
	GLuint mBufferId;
};

class OpenGL3RHIVertexBuffer : public RHIVertexBuffer
{
public:
	OpenGL3RHIVertexBuffer(size_t verticesCount, size_t vertexSize, RHIHardwareBufferUsage usage)
		: RHIVertexBuffer(verticesCount, vertexSize, usage) {
	}
};

class OpenGL3RHIIndexBuffer : public RHIIndexBuffer
{
public:
	OpenGL3RHIIndexBuffer(size_t indicesCount, RHIHardwareBufferUsage usage, RHIIndexBufferType indexType = RHIIndexBufferType::IBT_32)
		: RHIIndexBuffer(indicesCount, usage, indexType) {
	}

private:

};

#endif // ENGINE_OPENGL3RHI_OPENGL3RHIRESOURCE_H
