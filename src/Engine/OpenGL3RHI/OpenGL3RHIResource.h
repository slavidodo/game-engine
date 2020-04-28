
#ifndef ENGINE_OPENGL3RHI_OPENGL3RHIRESOURCE_H
#define ENGINE_OPENGL3RHI_OPENGL3RHIRESOURCE_H

#include "../RHI/RHIResource.h"

class OpenGL3RHIVertexBuffer;
class OpenGL3RHIIndexBuffer;
typedef std::shared_ptr<OpenGL3RHIVertexBuffer> OpenGL3RHIVertexBuffer_ptr;
typedef std::shared_ptr<OpenGL3RHIIndexBuffer> OpenGL3RHIIndexBuffer_ptr;

class OpenGLBufferImpl
{
public:
	OpenGLBufferImpl(size_t size, RHIHardwareBufferUsage usage, GLenum target)
		: mSize(size), mUsage(usage), mTarget(target)
	{
		mGlUsage = GetGLUsageForHardwareUsage(mUsage);
		glGenBuffers(1, &mBufferId);
	}

	~OpenGLBufferImpl() {
		if (mBufferId) {
			glDeleteBuffers(1, &mBufferId);
		}

		if (mLocked) {
			// there are cases where we lock the buffer
			// but map buffer is not supported, so we actually allocate the buffer
			// on the cpu and do a buffer subdata later
			// this is generally on opengl es 2.0

			// otherwise error: every Lock() must have a corresponding Unlock()
		}
	}

	OpenGLBufferImpl(OpenGLBufferImpl&) = delete;
	OpenGLBufferImpl& operator= (const OpenGLBufferImpl&) = delete;

	OBJECT_GETACCESSOR(GLuint, GLuint, BufferId);

	void* Lock(size_t size, size_t offset, RHIResourceLockMode lockMode, bool discard) {
		if (mLocked)
			return nullptr;

		mLocked = true;
		Bind();

		bool dynamicBuffer = mUsage == RHIHardwareBufferUsage::HWBU_STATIC;
		bool readOnly = lockMode == RHIResourceLockMode::RLM_READ_ONLY;

		// we prefer glMapBuffer here to range
		discard = discard || (!readOnly && size == mSize);
		if (discard) {
			glBufferData(mTarget, mSize, NULL, mGlUsage);
		}

		GLbitfield access;
		if (readOnly) {
			access |= GL_MAP_READ_BIT;
		} else {
			access |= GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
		}

		mLockedBuffer = glMapBufferRange(mTarget, offset, size, access);
		mLockedSize = size;
		mLockedOffset = offset;

		return mLockedBuffer;
	}

	void Unlock() {
		if (mLocked) {
			mLocked = false;
			Bind();

			glUnmapBuffer(mTarget);
			mLockedBuffer = NULL;
		}
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
	void Bind() {
		glBindBuffer(mTarget, mBufferId);
	}

	void* mLockedBuffer;
	size_t mSize;
	size_t mLockedSize = 0;
	size_t mLockedOffset = 0;
	GLuint mBufferId;
	RHIHardwareBufferUsage mUsage;
	GLenum mGlUsage;
	GLenum mTarget;
	bool mLocked = false;
};

class OpenGL3RHIVertexBuffer : public RHIVertexBuffer
{
public:
	OpenGL3RHIVertexBuffer(size_t verticesCount, size_t vertexSize, RHIHardwareBufferUsage usage)
		: RHIVertexBuffer(verticesCount, vertexSize, usage) {
		mBufferImpl.reset(new OpenGLBufferImpl(GL_ARRAY_BUFFER, usage, mSize));
	}

	void* Lock(size_t size, size_t offset, RHIResourceLockMode lockMode, bool discard) {
		return mBufferImpl->Lock(size, offset, lockMode, discard);
	}

	void Unlock() {
		mBufferImpl->Unlock();
	}

	GLuint GetBufferId() {
		return mBufferImpl->GetBufferId();
	}

private:
	std::unique_ptr<OpenGLBufferImpl> mBufferImpl;
};

class OpenGL3RHIIndexBuffer : public RHIIndexBuffer
{
public:
	OpenGL3RHIIndexBuffer(size_t indicesCount, RHIHardwareBufferUsage usage, RHIIndexBufferType indexType = RHIIndexBufferType::IBT_32)
		: RHIIndexBuffer(indicesCount, usage, indexType) {
		mBufferImpl.reset(new OpenGLBufferImpl(GL_ELEMENT_ARRAY_BUFFER, usage, mSize));
	}

	void* Lock(size_t size, size_t offset, RHIResourceLockMode lockMode, bool discard) {
		return mBufferImpl->Lock(size, offset, lockMode, discard);
	}

	void Unlock() {
		mBufferImpl->Unlock();
	}

	GLuint GetBufferId() {
		return mBufferImpl->GetBufferId();
	}

private:
	std::unique_ptr<OpenGLBufferImpl> mBufferImpl;
};


#endif // ENGINE_OPENGL3RHI_OPENGL3RHIRESOURCE_H
