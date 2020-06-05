
#ifndef ENGINE_OPENGLRHI_OPENGLRESOURCE_H
#define ENGINE_OPENGLRHI_OPENGLRESOURCE_H

#include "OpenGLDefinitions.h"
#include "OpenGLSpecs.h"
#include "../RHI/RHIResource.h"

class OpenGLVertexBuffer;
class OpenGLIndexBuffer;
class OpenGLUniformBuffer;

/// resource proxy
template<typename RHIType, typename TOpenGLType>
class TOpenGLResourceProxy : public RHIType
{
public:
	using RHITypeRef = TRefCountPtr<RHIType>;
	using OpenGLType = TOpenGLType;
	using OpenGLTypeRef = TRefCountPtr<OpenGLType>;
	using InitFuncType = std::function<OpenGLType*(RHIType*)>;

	TOpenGLResourceProxy(InitFuncType CreateFunc) {
		mResourceObject = CreateFunc(this);
	}

	OpenGLType* GetGLResourceObject() {
		return mResourceObject;
	}

private:
	OpenGLTypeRef mResourceObject;
};

template<typename RHIType, GLenum GLTypeEnum>
class OpenGLBufferImpl : public RHIType
{
public:
	static const GLenum TypeEnum = GLTypeEnum;
	typedef RHIType RHIBufferType;

	template<typename... Args>
	OpenGLBufferImpl(Args... args)
		: RHIType(std::forward<Args>(args)...)
	{
		mGlUsage = GetGLUsageForHardwareUsage(RHIType::mUsage);
		OpenGLSpecs::GenBuffers(1, &mResource);
	}

	~OpenGLBufferImpl() {
		if (mResource) {
			OpenGLSpecs::DeleteBuffers(1, &mResource);
		}

		if (mLocked) {
			// there are cases where we lock the buffer
			// but map buffer is not supported, so we actually allocate the buffer
			// on the cpu and do a buffer subdata later
			// this is generally on opengl es 2.0

			// otherwise error: every Lock() must have a corresponding Unlock()
		}
	}

	void* Lock(size_t Size, size_t MemberOffset, RHIResourceLockMode LockMode, bool Discard) {
		if (mLocked)
			return nullptr;

		mLocked = true;
		Bind();

		bool ReadOnly = LockMode == RHIResourceLockMode::RLM_READ_ONLY;

		// write only - full buffer, discard is available
		Discard = Discard || (!ReadOnly && Size == RHIType::mSize);
		if (Discard) {
			glBufferData(TypeEnum, RHIType::mSize, NULL, mGlUsage);
		}

		// todo: are mapped buffers always present?
		// opengl es 2.0 supports that only via an extention
		OpenGLSpecs::GLResourceLockMode GLLockMode = ReadOnly
			? OpenGLSpecs::GLResourceLockMode::RLM_ReadOnly
			: OpenGLSpecs::GLResourceLockMode::RLM_WriteOnly;
		mLockedBuffer = OpenGLSpecs::MapBufferRange(TypeEnum, MemberOffset, Size, GLLockMode);
		mLockedSize = Size;
		mLockedOffset = MemberOffset;

		return mLockedBuffer;
	}

	void Unlock() {
		if (mLocked) {
			mLocked = false;
			Bind();

			OpenGLSpecs::UnmapBuffer(TypeEnum);
			mLockedBuffer = nullptr;
		}
	}

	void Bind() {
		OpenGLSpecs::BindBuffer(TypeEnum, mResource);
	}

	GLuint GetGLResource() {
		return mResource;
	}

	static GLenum GetGLUsageForHardwareUsage(RHIHardwareBufferUsage HBUsage)
	{
		if (HBUsage & RHIHardwareBufferUsage::HWBU_DISCARD) {
			return GL_STREAM_DRAW;
		} else if (HBUsage & RHIHardwareBufferUsage::HWBU_STATIC) {
			return GL_STATIC_DRAW;
		} else {
			return GL_DYNAMIC_DRAW;
		}
	}

private:
	void* mLockedBuffer;
	size_t mLockedSize = 0;
	size_t mLockedOffset = 0;
	GLuint mResource;
	GLenum mGlUsage;
	bool mLocked = false;
};

class OpenGLVertexBuffer : public OpenGLBufferImpl<RHIVertexBuffer, GL_ARRAY_BUFFER>
{
public:
	OpenGLVertexBuffer(size_t verticesCount, size_t vertexSize, RHIHardwareBufferUsage usage)
		: OpenGLBufferImpl(verticesCount, vertexSize, usage) {

		if (!OpenGLSpecs::SupportsVertexAttribBinding() && usage & RHIHardwareBufferUsage::HWBU_ZERO_STRIDE) {
			mZeroStrideBuffer = malloc(mSize);
		}
	}

	~OpenGLVertexBuffer() {
		if (mZeroStrideBuffer) {
			free(mZeroStrideBuffer);
		}
	}

	void* GetZeroStrideBuffer() {
		return mZeroStrideBuffer;
	}

private:
	void* mZeroStrideBuffer = nullptr;
};

class OpenGLIndexBuffer : public OpenGLBufferImpl<RHIIndexBuffer, GL_ELEMENT_ARRAY_BUFFER>
{
public:
	OpenGLIndexBuffer(size_t indicesCount, RHIHardwareBufferUsage usage, RHIIndexBufferType indexType = RHIIndexBufferType::IBT_32)
		: OpenGLBufferImpl(indicesCount, usage, indexType) {}
};

class OpenGLUniformBuffer : public RHIUniformBuffer
{
public:
	GLuint Resource;
	uint32_t MemberOffset;
	uint32_t AllocatedSize;
	std::vector<RHIResource*> ResourceTable;

	// single-frame ubo
	bool StreamDraw;

	OpenGLUniformBuffer(const RHIUniformBufferLayout& InLayout)
		: RHIUniformBuffer(InLayout)
		, Resource(0)
		, MemberOffset(0)
		, AllocatedSize(0)
		, StreamDraw(false) {
	}

	void SetGLUniformBufferParams(GLuint InResource, uint32_t InOffset, uint32_t InAllocatedSize, bool InStreamDraw) {
		Resource = InResource;
		MemberOffset = InOffset;
		AllocatedSize = InAllocatedSize;
		StreamDraw = InStreamDraw;
	}
};

// Shaders
struct OpenGLShaderBindings
{
	uint8_t NumSamplers = 0;
	uint8_t NumUniformBuffers = 0;
	uint8_t NumUAVs = 0;

	std::vector<PackedArrayInfoVec> PackedUniformBuffers;
	PackedArrayInfoVec PackedGlobalArrays;

	friend bool operator==(const OpenGLShaderBindings& A, const OpenGLShaderBindings& B)
	{
		bool Equal = true;

		//bEqual &= A.InOutMask == B.InOutMask;
		Equal &= A.NumSamplers == B.NumSamplers;
		Equal &= A.NumUniformBuffers == B.NumUniformBuffers;
		Equal &= A.NumUAVs == B.NumUAVs;
		//bEqual &= A.bFlattenUB == B.bFlattenUB;
		Equal &= A.PackedGlobalArrays.size() == B.PackedGlobalArrays.size();
		Equal &= A.PackedUniformBuffers.size() == B.PackedUniformBuffers.size();
		//bEqual &= A.InputVaryings.Num() == B.InputVaryings.Num();
		//bEqual &= A.OutputVaryings.Num() == B.OutputVaryings.Num();
		//bEqual &= A.ShaderResourceTable == B.ShaderResourceTable;

		if (!Equal) {
			return Equal;
		}

		Equal &= memcmp(A.PackedGlobalArrays.data(), B.PackedGlobalArrays.data(),
						sizeof(PackedArrayInfo) * A.PackedGlobalArrays.size()) == 0;

		for (size_t Item = 0; Item < A.PackedUniformBuffers.size(); Item++) {
			const PackedArrayInfoVec& VecA = A.PackedUniformBuffers[Item];
			const PackedArrayInfoVec& VecB = B.PackedUniformBuffers[Item];
		
			Equal &= memcmp(VecA.data(), VecB.data(), sizeof(PackedArrayInfo) * VecA.size()) == 0;
		}

		//for (int32 Item = 0; bEqual && Item < A.InputVaryings.Num(); Item++) {
		//	bEqual &= A.InputVaryings[Item] == B.InputVaryings[Item];
		//}

		//for (int32 Item = 0; bEqual && Item < A.OutputVaryings.Num(); Item++) {
		//	bEqual &= A.OutputVaryings[Item] == B.OutputVaryings[Item];
		//}

		return Equal;
	}
};

template<GLenum TTypeEnum, ShaderFrequency TFrequency>
class TOpenGLShader : public RefCountedObject
{
public:
	static const GLenum TypeEnum = TTypeEnum;
	static const ShaderFrequency Frequency = TFrequency;

	OpenGLShaderBindings Bindings;
	std::vector<UniformBufferCopyInfo> UniformBuffersCopyInfo;

	TOpenGLShader(GLuint InGlResource)
		: mGLResource(InGlResource) {}

	GLuint GetGLResource() {
		return mGLResource;
	}

private:
	GLuint mGLResource;
};

typedef TOpenGLShader<GL_VERTEX_SHADER, ShaderFrequency::SF_Vertex> OpenGLVertexShader;
typedef TOpenGLShader<GL_FRAGMENT_SHADER, ShaderFrequency::SF_Pixel> OpenGLPixelShader;

typedef TOpenGLResourceProxy<RHIVertexShader, OpenGLVertexShader> OpenGLVertexShaderProxy;
typedef TOpenGLResourceProxy<RHIPixelShader, OpenGLPixelShader> OpenGLPixelShaderProxy;

/// Shader Bindings
class OpenGLVertexDeclaration;
class OpenGLLinkedProgram;
class OpenGLBoundShaderState;

class OpenGLShaderParameterCache
{
public:
	OpenGLShaderParameterCache();
	~OpenGLShaderParameterCache();

	void InitializeResources(int32_t UniformArraySize);

	void MarkAllDirty();
	void Set(uint32_t BufferIndexName, uint32_t ByteOffset, uint32_t NumBytes, const void* NewValues);

	void CommitPackedGlobals(const OpenGLLinkedProgram* LinkedProgram, int32_t Stage);
	void CommitPackedUniformBuffers(OpenGLLinkedProgram* LinkedProgram, int32_t Stage, RHIUniformBufferRef* UniformBuffers, const std::vector<UniformBufferCopyInfo>& UniformBuffersCopyInfo);

private:
	uint8_t* PackedGlobalUniforms[PACKED_TYPEINDEX_MAX];
	uint8_t* PackedUniformsScratch[PACKED_TYPEINDEX_MAX];

	struct FRange
	{
		uint32_t StartVector;
		uint32_t NumVectors;

		void MarkDirtyRange(uint32_t NewStartVector, uint32_t NewNumVectors) {
			if (NumVectors > 0) {
				uint32_t MinVector = std::min<uint32_t>(StartVector, NewStartVector);
				uint32_t MaxVector = std::max<uint32_t>(StartVector + NumVectors, NewStartVector + NewNumVectors);

				StartVector = MinVector;
				NumVectors = (MaxVector - MinVector) + 1;
			} else {
				StartVector = NewStartVector;
				NumVectors = NewNumVectors;
			}
		}
	};
	FRange	PackedGlobalUniformDirty[PACKED_TYPEINDEX_MAX]; // dirty ranges

	int32_t GlobalUniformArraySize;
};

struct OpenGLVertexElement
{
	GLenum Type = 0;
	GLuint StreamIndex = 0;
	GLuint MemberOffset = 0;
	GLuint Size = 0;
	GLuint Divisor = 0;
	GLuint HashStride = 0;
	uint8_t Normalized = 0;
	uint8_t AttributeIndex = 0;
	uint8_t ShouldConvertToFloat = 0;
	uint8_t Padding = 0;
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

class OpenGLLinkedProgramConfiguration
{
public:
	struct ShaderInfo
	{
		OpenGLShaderBindings Bindings;
		GLuint Resource;
	}
	Shaders[ShaderStage::NUM_SHADER_STAGES];

	OpenGLLinkedProgramConfiguration()
	{
		for (int32_t Stage = 0; Stage < ShaderStage::NUM_SHADER_STAGES; Stage++) {
			Shaders[Stage].Resource = 0;
		}
	}

	friend bool operator ==(const OpenGLLinkedProgramConfiguration& A, const OpenGLLinkedProgramConfiguration& B)
	{
		bool bEqual = true;
		for (int32_t Stage = 0; Stage < ShaderStage::NUM_SHADER_STAGES && bEqual; Stage++) {
			bEqual &= A.Shaders[Stage].Resource == B.Shaders[Stage].Resource;
			bEqual &= A.Shaders[Stage].Bindings == B.Shaders[Stage].Bindings;
		}
		return bEqual;
	}
};

class OpenGLLinkedProgram
{
public:
	struct PackedUniformInfo
	{
		GLint Location;
		uint8_t ArrayType;
		uint8_t Index;

		void operator=(const PackedUniformInfo& Rhs) {
			Location = Rhs.Location;
			ArrayType = Rhs.ArrayType;
			Index = Rhs.Index;
		}
	};

	using PackedUniformInfoVec = std::vector<PackedUniformInfo>;

	// per-stage globals and uniform buffers
	struct StagePackedUniformInfo
	{
		PackedUniformInfoVec PackedUniforms; // regular globals (opengl only)
		std::vector<PackedUniformInfoVec> PackedUniformBuffers; // constant buffers

		std::vector<uint32_t> LastEmulatedUniformBufferSet;
	};

	StagePackedUniformInfo StagePackedUniformInfo[ShaderStage::NUM_SHADER_STAGES];
	OpenGLLinkedProgramConfiguration Config;
	GLuint Program;

	OpenGLLinkedProgram(OpenGLLinkedProgramConfiguration Config, GLuint Program)
		: Config(Config), Program(Program) {}

	void VerifyUniformBlockBindings(int Stage, uint32_t FirstUniformBuffer);
	void ConfigureShaderStage(int Stage, uint32_t FirstUniformBuffer);
	static inline void SortPackedUniformInfos(const PackedUniformInfoVec& ReflectedUniformInfos, const PackedArrayInfoVec& PackedGlobalArrays, PackedUniformInfoVec& OutPackedUniformInfos);
};

class OpenGLBoundShaderState : public RHIBoundShaderState
{
public:
	OpenGLBoundShaderState(
		OpenGLLinkedProgram* LinkedProgram,
		OpenGLVertexDeclaration* VertexDeclaration,
		RHIVertexShader* VertexShaderRHI,
		RHIPixelShader* PixelShaderRHI);

	OpenGLLinkedProgram* GetLinkedProgram() {
		return mLinkedProgram;
	}
	OpenGLVertexDeclaration* GetVertexDeclaration() {
		return mVertexDeclaration;
	}
	OpenGLVertexShader* GetVertexShader() {
		return mVertexShaderProxy->GetGLResourceObject();
	}
	OpenGLPixelShader* GetPixelShader() {
		return mPixelShaderProxy->GetGLResourceObject();
	}

	void GetNumUniformBuffers(int32_t NumUniformBuffers[SF_Compute]) {
		// todo: check syncing
		NumUniformBuffers[SF_Vertex] = GetVertexShader()->Bindings.NumUniformBuffers;
		NumUniformBuffers[SF_Pixel] = GetPixelShader()->Bindings.NumUniformBuffers;
	}

private:
	OpenGLLinkedProgram* mLinkedProgram;
	TRefCountPtr<OpenGLVertexDeclaration> mVertexDeclaration;
	TRefCountPtr<OpenGLVertexShaderProxy> mVertexShaderProxy;
	TRefCountPtr<OpenGLPixelShaderProxy> mPixelShaderProxy;
};

template<class T>
struct TOpenGLResourceValidator
{
};

template<> struct TOpenGLResourceValidator<RHIVertexBuffer>
{
	typedef OpenGLVertexBuffer ResourceType;
};

template<> struct TOpenGLResourceValidator<RHIIndexBuffer>
{
	typedef OpenGLIndexBuffer ResourceType;
};

template<> struct TOpenGLResourceValidator<RHIUniformBuffer>
{
	typedef OpenGLUniformBuffer ResourceType;
};

template<> struct TOpenGLResourceValidator<RHIVertexShader>
{
	typedef OpenGLVertexShaderProxy ResourceType;
};

template<> struct TOpenGLResourceValidator<RHIPixelShader>
{
	typedef OpenGLPixelShaderProxy ResourceType;
};

template<> struct TOpenGLResourceValidator<RHIVertexDeclaration>
{
	typedef OpenGLVertexDeclaration ResourceType;
};

template<> struct TOpenGLResourceValidator<RHIBoundShaderState>
{
	typedef OpenGLBoundShaderState ResourceType;
};

template<class TRHIResourceType, class TOpenGLResourceType = typename TOpenGLResourceValidator<TRHIResourceType>::ResourceType>
inline TOpenGLResourceType* StaticResourceCast(TRHIResourceType* InResource)
{
	return static_cast<TOpenGLResourceType*>(InResource);
}

#endif // ENGINE_OPENGLRHI_OPENGLRESOURCE_H
