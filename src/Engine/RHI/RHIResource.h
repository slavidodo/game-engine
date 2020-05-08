
#ifndef ENGINE_RHI_RHIRESOURCE_H
#define ENGINE_RHI_RHIRESOURCE_H

class RHIResource;
typedef std::unique_ptr<RHIResource> Resource_uPtr;
typedef std::shared_ptr<RHIResource> Resource_ptr;
typedef std::weak_ptr<RHIResource> Resource_weakPtr;

enum class RHIResourceLockMode : uint8_t
{
	RLM_NORMAL,
	RLM_READ_ONLY,
	RLM_WRITE_ONLY,
	RLM_WRITE_NO_OVERWRITE,
};

enum RHIHardwareBufferUsage : uint8_t
{
	// we could specify general uses, such as STREAM_DRAW
	HWBU_STATIC = 1 << 0, // to be filled once
	HWBU_DYNAMIC = 1 << 2, // to be modified more often
	HWBU_DISCARD = 1 << 3, // to be streamed every frame

	// other uses with buffer data
	HWBU_WRITE_ONLY = 1 << 4, // contents are not read back
	HWBU_READ_ONLY = 1 << 5, // contents are to be read from the gpu, often RT usage

	HWBU_STATIC_WRITE_ONLY = HWBU_STATIC | HWBU_WRITE_ONLY, // optimal buffer usage
	HWBU_DYNAMIC_WRITE_ONLY = HWBU_DYNAMIC | HWBU_WRITE_ONLY, // write only dynamic

	HWBU_DYNAMIC_WRITE_ONLY_DISCARD = HWBU_DYNAMIC_WRITE_ONLY | HWBU_DISCARD, // dyanmic, write only discardable buffer for regular refilling
};

enum RHIIndexBufferType {
	IBT_16,
	IBT_32,
};

enum VertexElementType
{
	VET_None,
	VET_Float1,
	VET_Float2,
	VET_Float3,
	VET_Float4,
	VET_PackedNormal,	// FPackedNormal
	VET_UByte4,
	VET_UByte4N,
	VET_Color,
	VET_Short2,
	VET_Short4,
	VET_Short2N,		// 16 bit word normalized to (value/32767.0,value/32767.0,0,0,1)
	VET_Half2,			// 16 bit float using 1 bit sign, 5 bit exponent, 10 bit mantissa 
	VET_Half4,
	VET_Short4N,		// 4 X 16 bit word, normalized 
	VET_UShort2,
	VET_UShort4,
	VET_UShort2N,		// 16 bit word normalized to (value/65535.0,value/65535.0,0,0,1)
	VET_UShort4N,		// 4 X 16 bit word unsigned, normalized 
	VET_URGB10A2N,		// 10 bit r, g, b and 2 bit a normalized to (value/1023.0f, value/1023.0f, value/1023.0f, value/3.0f)
	VET_UInt,
	VET_MAX,
};

enum class RasterizerFillMode
{
	FM_Point,
	FM_Wireframe,
	FM_Solid,
};

enum class RasterizerCullMode
{
	CM_None,
	CM_ClockWise,
	CM_CounterClockWise,
};

class RHIResource : std::enable_shared_from_this<RHIResource>
{
public:

protected:
	RHIResource() = default;
	virtual ~RHIResource() = default;

private:
};

// Vertex elements
struct VertexElement
{
	uint8_t StreamIndex;
	uint8_t Offset;
	VertexElementType Type;
	uint8_t AttributeIndex;
	uint16_t Stride;
	/**
	 * Whether to use instance index or vertex index to consume the element.
	 * eg if bUseInstanceIndex is 0, the element will be repeated for every instance.
	 */
	bool UseInstanceIndex;

	VertexElement() {}
	VertexElement(uint8_t streamIndex, uint8_t offset, VertexElementType type, uint8_t attributeIndex, uint16_t stride, bool useInstanceIndex = false) :
		StreamIndex(streamIndex),
		Offset(offset),
		Type(type),
		AttributeIndex(attributeIndex),
		Stride(stride),
		UseInstanceIndex(useInstanceIndex)
	{}

	void operator=(const VertexElement& Other)
	{
		StreamIndex = Other.StreamIndex;
		Offset = Other.Offset;
		Type = Other.Type;
		AttributeIndex = Other.AttributeIndex;
		UseInstanceIndex = Other.UseInstanceIndex;
	}
};

// Buffers
class RHIHardwareBuffer;
class RHIVertexBuffer;
class RHIIndexBuffer;
class RHIUniformBuffer;
class RHIStructuredBuffer;
typedef std::shared_ptr<RHIHardwareBuffer> RHIHardwareBuffer_ptr;
typedef std::shared_ptr<RHIVertexBuffer> RHIVertexBuffer_ptr;
typedef std::shared_ptr<RHIIndexBuffer> RHIIndexBuffer_ptr;
typedef std::shared_ptr<RHIUniformBuffer> RHIUniformBuffer_ptr;
typedef std::shared_ptr<RHIStructuredBuffer> RHIStructuredBuffer_ptr;

class RHIHardwareBuffer : public RHIResource
{
public:
	RHIHardwareBuffer(RHIHardwareBufferUsage usage)
		: mUsage(usage), mSize(0) {}

	RHIHardwareBuffer(RHIHardwareBuffer&) = delete; // delete copy constructor

	OBJECT_GETACCESSOR(RHIHardwareBufferUsage, RHIHardwareBufferUsage, Usage);
	OBJECT_GETACCESSOR(size_t, size_t, Size);

	bool CanBeDiscarded() const {
		return (mUsage & RHIHardwareBufferUsage::HWBU_DISCARD) != 0
			|| (mUsage & RHIHardwareBufferUsage::HWBU_DYNAMIC) != 0;
	}

protected:
	RHIHardwareBufferUsage mUsage;
	size_t mSize;
};

class RHIVertexBuffer : public RHIHardwareBuffer
{
public:
	RHIVertexBuffer(size_t verticesCount, size_t vertexSize, RHIHardwareBufferUsage usage)
		: RHIHardwareBuffer(usage), mVerticesCount(verticesCount), mVertexSize(vertexSize) {
		mSize = verticesCount * vertexSize;
	}

	OBJECT_GETACCESSOR(size_t, size_t, VerticesCount);
	OBJECT_GETACCESSOR(size_t, size_t, VertexSize);

private:
	size_t mVerticesCount;
	size_t mVertexSize;
};

class RHIIndexBuffer : public RHIHardwareBuffer
{
public:
	RHIIndexBuffer(size_t indicesCount, RHIHardwareBufferUsage usage, RHIIndexBufferType indexType = RHIIndexBufferType::IBT_32)
		: RHIHardwareBuffer(usage), mIndicesCount(indicesCount) {
		mIndexSize = indexType == RHIIndexBufferType::IBT_16
			? sizeof(uint16_t)
			: sizeof(uint32_t);
		mSize = indicesCount * mIndexSize;
	}

	OBJECT_GETACCESSOR(size_t, size_t, IndicesCount);
	OBJECT_GETACCESSOR(size_t, size_t, IndexSize);

private:
	size_t mIndicesCount;
	size_t mIndexSize;
};

class RHIUniformBuffer : public RHIHardwareBuffer
{
public:
private:
};

class RHIStructuredBuffer : public RHIHardwareBuffer
{
public:
private:
};

/// Textures
class RHITexture;
class RHITexture2D;
class RHITexture3D;
typedef std::shared_ptr<RHITexture> RHITexture_ptr;
typedef std::shared_ptr<RHITexture2D> RHITexture2D_ptr;
typedef std::shared_ptr<RHITexture3D> RHITexture3D_ptr;

class RHITexture : public RHIResource
{
public:
	virtual RHITexture2D* GetTexture2D() { return nullptr; }
	virtual RHITexture3D* GetTexture3D() { return nullptr; }

private:
};

class RHITexture2D : public RHITexture
{
public:
	RHITexture2D(uint32_t width, uint32_t height)
		: RHITexture(), mWidth(width), mHeight(height) {}

	RHITexture2D* GetTexture2D() override final { return this; }

	inline glm::uvec2 GetSize() {
		return glm::fvec2(mWidth, mHeight);
	}

	inline uint32_t GetWidth() { return mWidth; }
	inline uint32_t GetHeight() { return mHeight; }

private:
	uint32_t mWidth;
	uint32_t mHeight;
};

class RHITexture3D : public RHITexture
{
public:
	RHITexture3D(uint32_t width, uint32_t height, uint32_t depth)
		: RHITexture(), mWidth(width), mHeight(height), mDepth(depth) {}

	RHITexture3D* GetTexture3D() override final { return this; }

	inline glm::uvec2 GetSize() {
		return glm::fvec2(mWidth, mHeight);
	}

	inline uint32_t GetWidth() { return mWidth; }
	inline uint32_t GetHeight() { return mHeight; }
	inline uint32_t GetDepth() { return mDepth; }

private:
	uint32_t mWidth;
	uint32_t mHeight;
	uint32_t mDepth;
};

/// State Blocks
struct RasterizerStateInitializerRHI
{
	RasterizerFillMode FillMode;
	RasterizerCullMode CullMode;
	float DepthBias;
	float SlopeScaleDepthBias;
	bool bAllowMSAA;
	bool bEnableLineAA;
};

class RHISamplerState : public RHIResource
{
public:
	virtual bool IsImmutable() const { return false; }
};

class RHIRasterizerState : public RHIResource
{
public:
	virtual bool GetInitializer(struct RasterizerStateInitializerRHI& Init) { return false; }
};

/// Shader Bindings
class RHIVertexDeclaration;
class RHIBoundShaderState;
typedef std::shared_ptr<RHIVertexDeclaration> RHIVertexDeclaration_ptr;
typedef std::shared_ptr<RHIBoundShaderState> RHIBoundShaderState_ptr;

typedef std::vector<struct VertexElement> VertexDeclarationElementList;
class RHIVertexDeclaration : public RHIResource
{
public:
	virtual bool GetInitializer(VertexDeclarationElementList& Init) { return false; }
};

class RHIBoundShaderState : public RHIResource {};

/// Shaders
// this is using DirectX naming convention
class RHIShader;
class RHIVertexShader;
class RHIHullShader; // [OGL] TCS
class RHIDomainShader; // [OGL] TES
class RHIPixelShader; // [OGL] Fragment shader
class RHIGeometryShader;
typedef std::shared_ptr<RHIShader> RHIShader_ptr;
typedef std::shared_ptr<RHIVertexShader> RHIVertexShader_ptr;
typedef std::shared_ptr<RHIHullShader> RHIHullShader_ptr;
typedef std::shared_ptr<RHIDomainShader> RHIDomainShader_ptr;
typedef std::shared_ptr<RHIPixelShader> RHIPixelShader_ptr;
typedef std::shared_ptr<RHIGeometryShader> RHIGeometryShader_ptr;

class RHIShader : public RHIResource {};

class RHIVertexShader : public RHIShader {};
class RHIHullShader : public RHIShader {};
class RHIDomainShader : public RHIShader {};
class RHIPixelShader : public RHIShader {};
class RHIGeometryShader : public RHIShader {};

/// Pipelines
class RHIGraphicsPipelineState;
class RHIGraphicsPipelineStateFallBack;
typedef std::shared_ptr<RHIGraphicsPipelineState> RHIGraphicsPipelineState_ptr;
typedef std::shared_ptr<RHIGraphicsPipelineStateFallBack> RHIGraphicsPipelineStateFallBack_ptr;

class RHIGraphicsPipelineState : public RHIResource {};
class RHIGraphicsPipelineStateFallBack : public RHIGraphicsPipelineState {};

/// Render Target

/*
 * TODO: in the context here, we refer to multipass rendering and using multiple
 * render targets, atm we are just defining a single render target bound to a
 * texture, which in our case is where the drawn data will be
 */
struct RHIRenderPassInfo
{
	RHITexture_ptr RenderTarget;

	explicit RHIRenderPassInfo(RHITexture_ptr InRenderTarget) {
		RenderTarget = InRenderTarget;
	}

	explicit RHIRenderPassInfo() = default;
};

#endif // ENGINE_RHI_RHIRESOURCE_H
