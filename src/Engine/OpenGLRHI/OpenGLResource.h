
#ifndef ENGINE_OPENGLRHI_OPENGLRESOURCE_H
#define ENGINE_OPENGLRHI_OPENGLRESOURCE_H

#include "../RHI/RHIResource.h"

class OpenGLVertexBuffer;
class OpenGLIndexBuffer;
typedef std::shared_ptr<OpenGLVertexBuffer> OpenGLVertexBuffer_ptr;
typedef std::shared_ptr<OpenGLIndexBuffer> OpenGLIndexBuffer_ptr;

/// resource proxy
template<typename RHIType, typename TOpenGLType>
class TOpenGLResourceProxy : public RHIType, std::enable_shared_from_this<TOpenGLResourceProxy<RHIType, TOpenGLType>>
{
public:
	using RHITypeRef = std::shared_ptr<RHIType>;
	using OpenGLType = TOpenGLType;
	using OpenGLTypeRef = std::shared_ptr<OpenGLType>;
	using InitFuncType = std::function<OpenGLTypeRef(RHIType*)>;

	TOpenGLResourceProxy(InitFuncType CreateFunc) {
		mResourceObject = CreateFunc(this);
	}

	OpenGLTypeRef GetGLResourceObject() {
		return mResourceObject;
	}

private:
	OpenGLTypeRef mResourceObject;
};

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
			access = GL_MAP_READ_BIT;
		} else {
			access = GL_MAP_WRITE_BIT;

			// hint that the whole buffer is to be discarded!
			if (discard) {
				access |= GL_MAP_INVALIDATE_BUFFER_BIT;
			}
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
			mLockedBuffer = nullptr;
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

class OpenGLVertexBuffer : public RHIVertexBuffer
{
public:
	OpenGLVertexBuffer(size_t verticesCount, size_t vertexSize, RHIHardwareBufferUsage usage)
		: RHIVertexBuffer(verticesCount, vertexSize, usage) {
		mBufferImpl.reset(new OpenGLBufferImpl(mSize, usage, GL_ARRAY_BUFFER));
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

class OpenGLIndexBuffer : public RHIIndexBuffer
{
public:
	OpenGLIndexBuffer(size_t indicesCount, RHIHardwareBufferUsage usage, RHIIndexBufferType indexType = RHIIndexBufferType::IBT_32)
		: RHIIndexBuffer(indicesCount, usage, indexType) {
		mBufferImpl.reset(new OpenGLBufferImpl(mSize, usage, GL_ELEMENT_ARRAY_BUFFER));
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

// Shaders
template<GLenum GLTypeEnum>
class TOpenGLShader
{
public:
	static const GLenum TypeEnum = GLTypeEnum;

	TOpenGLShader(GLuint InGlResource)
		: GLResource(InGlResource) {}

	GLuint GetGLResource() {
		return GLResource;
	}

private:
	GLuint GLResource;
};

typedef TOpenGLShader<GL_VERTEX_SHADER> OpenGLVertexShader;
typedef TOpenGLShader<GL_FRAGMENT_SHADER> OpenGLPixelShader;
typedef std::shared_ptr<OpenGLVertexShader> OpenGLVertexShader_ptr;
typedef std::shared_ptr<OpenGLPixelShader> OpenGLPixelShader_ptr;

typedef TOpenGLResourceProxy<RHIVertexShader, OpenGLVertexShader> OpenGLVertexShaderProxy;
typedef TOpenGLResourceProxy<RHIPixelShader, OpenGLPixelShader> OpenGLPixelShaderProxy;
typedef std::shared_ptr<OpenGLVertexShaderProxy> OpenGLVertexShaderProxy_ptr;
typedef std::shared_ptr<OpenGLPixelShaderProxy> OpenGLPixelShaderProxy_ptr;

/// Shader Bindings
class OpenGLVertexDeclaration;
class OpenGLLinkedProgram;
class OpenGLBoundShaderState;
typedef std::shared_ptr<OpenGLVertexDeclaration> OpenGLVertexDeclaration_ptr;
typedef std::shared_ptr<OpenGLLinkedProgram> OpenGLLinkedProgram_ptr;
typedef std::shared_ptr<OpenGLBoundShaderState> OpenGLBoundShaderState_ptr;

struct OpenGLVertexElement
{
	GLenum Type;
	GLuint StreamIndex;
	GLuint Offset;
	GLuint Size;
	GLuint Divisor;
	GLuint HashStride;
	uint8_t Normalized;
	uint8_t AttributeIndex;
	uint8_t ShouldConvertToFloat;
	uint8_t Padding;

	OpenGLVertexElement()
		: Padding(0)
	{
	}
};

typedef std::vector<OpenGLVertexElement> OpenGLVertexElements;
class OpenGLVertexDeclaration : public RHIVertexDeclaration
{
public:
	enum
	{
		MaxVertexElementCount = 16,
	};

	OpenGLVertexElements VertexElements;

	uint16_t StreamStrides[MaxVertexElementCount];

	OpenGLVertexDeclaration(const OpenGLVertexElements& elements, const uint16_t* strides)
		: VertexElements(elements)
	{
		memcpy(StreamStrides, strides, sizeof(strides));
	}

	virtual bool GetInitializer(VertexDeclarationElementList& Init) override final;
};

class OpenGLLinkedProgram
{
public:
	OpenGLLinkedProgram(GLuint Program)
		: mProgram(Program) {}

	GLuint GetProgram() {
		return mProgram;
	}

private:
	GLuint mProgram;
};

class OpenGLBoundShaderState : public RHIBoundShaderState
{
public:
	OpenGLBoundShaderState(
		OpenGLLinkedProgram_ptr LinkedProgram,
		OpenGLVertexDeclaration_ptr VertexDeclaration,
		RHIVertexShader_ptr VertexShaderRHI,
		RHIPixelShader_ptr PixelShaderRHI)
		: mLinkedProgram(LinkedProgram), mVertexDeclaration(VertexDeclaration) {

		mVertexShaderProxy = std::static_pointer_cast<OpenGLVertexShaderProxy>(VertexShaderRHI);
		mPixelShaderProxy = std::static_pointer_cast<OpenGLPixelShaderProxy>(PixelShaderRHI);
	}

	OpenGLLinkedProgram_ptr GetLinkedProgram() {
		return mLinkedProgram;
	}
	OpenGLVertexDeclaration_ptr GetVertexDeclaration() {
		return mVertexDeclaration;
	}
	OpenGLVertexShader_ptr GetVertexShader() {
		return mVertexShaderProxy->GetGLResourceObject();
	}
	OpenGLPixelShader_ptr GetPixelShader() {
		return mPixelShaderProxy->GetGLResourceObject();
	}

private:
	OpenGLLinkedProgram_ptr mLinkedProgram;
	OpenGLVertexDeclaration_ptr mVertexDeclaration;
	OpenGLVertexShaderProxy_ptr mVertexShaderProxy;
	OpenGLPixelShaderProxy_ptr mPixelShaderProxy;
};


#endif // ENGINE_OPENGLRHI_OPENGLRESOURCE_H
