
#ifndef ENGINE_RHI_RHIRESOURCE_H
#define ENGINE_RHI_RHIRESOURCE_H

#include "RHIDefinitions.h"

class RHIResource : public ThreadSafeRefCountedObject
{
protected:
	RHIResource() = default;
	virtual ~RHIResource() = default;
};

// Vertex elements
struct VertexElement
{
	uint8_t StreamIndex;
	uint8_t MemberOffset;
	VertexElementType Type;
	uint8_t AttributeIndex;
	uint16_t Stride;
	/**
	 * Whether to use instance index or vertex index to consume the element.
	 * eg if bUseInstanceIndex is 0, the element will be repeated for every instance.
	 */
	bool UseInstanceIndex;

	VertexElement()
		: StreamIndex(0)
		, MemberOffset(0)
		, Type(VertexElementType::VET_None)
		, AttributeIndex(0)
		, Stride(0)
	{}

	VertexElement(uint8_t streamIndex, uint8_t offset, VertexElementType type, uint8_t attributeIndex, uint16_t stride, bool useInstanceIndex = false) :
		StreamIndex(streamIndex),
		MemberOffset(offset),
		Type(type),
		AttributeIndex(attributeIndex),
		Stride(stride),
		UseInstanceIndex(useInstanceIndex)
	{}

	void operator=(const VertexElement& Other)
	{
		StreamIndex = Other.StreamIndex;
		MemberOffset = Other.MemberOffset;
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

class RHIHardwareBuffer : public RHIResource
{
public:
	RHIHardwareBuffer(RHIHardwareBufferUsage usage)
		: mUsage(usage), mSize(0) {}

	RHIHardwareBuffer(RHIHardwareBuffer&) = delete; // delete copy constructor

	RHIHardwareBufferUsage GetUsage() { return mUsage; }
	size_t GetSize() { return mSize; }

	bool IsDynamicBuffer() const {
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

	size_t GetVerticesCount() { return mVerticesCount; }
	size_t GetVertexSize() { return mVertexSize; }

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

	size_t GetIndicesCount() const { return mIndicesCount; }
	size_t GetIndexSize() const { return mIndexSize; }

private:
	size_t mIndicesCount;
	size_t mIndexSize;
};

struct RHIUniformBufferLayout
{
	struct ResourceParameter
	{
		uint16_t MemberOffset;
		EUniformBufferBaseType MemberType;
	};

	size_t ConstantBufferSize = 0;
	std::vector<ResourceParameter> Resources;
};

class RHIUniformBuffer : public RHIResource
{
public:
	RHIUniformBuffer(const RHIUniformBufferLayout& InLayout)
		: mLayout(&InLayout), mLayourConstantBufferSize(InLayout.ConstantBufferSize)
	{
	}

	uint32_t GetSize() const {
		return mLayourConstantBufferSize;
	}

	const RHIUniformBufferLayout& GetLayout() const { return *mLayout; }

private:
	const RHIUniformBufferLayout* mLayout;
	size_t mLayourConstantBufferSize;
};

class RHIStructuredBuffer : public RHIHardwareBuffer
{
public:
private:
};

/// Textures
class RHITexture : public RHIResource
{
public:
	RHITexture(uint32_t InNumMips, uint32_t InNumSamples, EPixelFormat InFormat, uint32_t InFlags)
		: mNumMips(InNumMips)
		, mNumSamples(InNumSamples)
		, mFormat(InFormat)
		, mFlags(InFlags)
	{}

	virtual RHITexture2D* GetTexture2D() { return nullptr; }
	virtual RHITexture2DArray* GetTexture2DArray() { return nullptr; }
	virtual RHITexture3D* GetTexture3D() { return nullptr; }
	virtual RHITextureCube* GetTextureCube() { return nullptr; }
	virtual RHITextureReference* GetTextureReference() { return nullptr; }

	virtual glm::uvec3 GetSizeXYZ() const = 0;

	virtual void* GetNativeResource() const {
		// Override this in derived classes to expose access to the native texture resource
		return nullptr;
	}

	virtual void* GetNativeShaderResourceView() const {
		// Override this in derived classes to expose access to the native texture resource
		return nullptr;
	}

	virtual void* GetTextureBaseRHI() {
		// Override this in derived classes to expose access to the native texture resource
		return nullptr;
	}

	/** @return The number of mip-maps in the texture. */
	uint32_t GetNumMips() const { return mNumMips; }

	/** @return The format of the pixels in the texture. */
	EPixelFormat GetFormat() const { return mFormat; }

	/** @return The flags used to create the texture. */
	uint32_t GetFlags() const { return mFlags; }

	/* @return the number of samples for multi-sampling. */
	uint32_t GetNumSamples() const { return mNumSamples; }

	/** @return Whether the texture is multi sampled. */
	bool IsMultisampled() const { return mNumSamples > 1; }

private:
	uint32_t mNumMips;
	uint32_t mNumSamples;
	uint32_t mFlags;
	EPixelFormat mFormat;
};

class RHITexture2D : public RHITexture
{
public:
	RHITexture2D(uint32_t InSizeX, uint32_t InSizeY, uint32_t InNumMips, uint32_t InNumSamples, EPixelFormat InFormat, uint32_t InFlags)
		: RHITexture(InNumMips, InNumSamples, InFormat, InFlags)
		, mSizeX(InSizeX)
		, mSizeY(InSizeY)
	{}

	virtual RHITexture2D* GetTexture2D() override { return this; }

	uint32_t GetSizeX() const { return mSizeX; }
	uint32_t GetSizeY() const { return mSizeY; }

	inline glm::uvec2 GetSizeXY() const {
		return glm::uvec2(mSizeX, mSizeY);
	}

	glm::uvec3 GetSizeXYZ() const override{
		return glm::uvec3(mSizeX, mSizeY, 1);
	}

private:
	uint32_t mSizeX;
	uint32_t mSizeY;
};

class RHITexture2DArray : public RHITexture2D
{
public:
	RHITexture2DArray(uint32_t InSizeX, uint32_t InSizeY, uint32_t InSizeZ, uint32_t InNumMips, uint32_t InNumSamples, EPixelFormat InFormat, uint32_t InFlags)
		: RHITexture2D(InSizeX, InSizeY, InNumMips, InNumSamples, InFormat, InFlags)
		, mSizeZ(InSizeZ)
	{}

	virtual RHITexture2DArray* GetTexture2DArray() override { return this; }
	virtual RHITexture2D* GetTexture2D() override { return nullptr; }

	uint32_t GetSizeZ() { return mSizeZ; }

	virtual glm::uvec3 GetSizeXYZ() const override {
		return glm::uvec3(GetSizeX(), GetSizeY(), mSizeZ);
	}

private:
	uint32_t mSizeZ;
};

class RHITexture3D : public RHITexture
{
public:
	RHITexture3D(uint32_t InSizeX, uint32_t InSizeY, uint32_t InSizeZ, uint32_t InNumMips, uint32_t InNumSamples, EPixelFormat InFormat, uint32_t InFlags)
		: RHITexture(InNumMips, InNumSamples, InFormat, InFlags)
		, mSizeX(InSizeX)
		, mSizeY(InSizeY)
		, mSizeZ(InSizeZ)
	{}

	RHITexture3D* GetTexture3D() override final { return this; }

	inline uint32_t GetSizeX() { return mSizeX; }
	inline uint32_t GetSizeY() { return mSizeY; }
	inline uint32_t GetSizeZ() { return mSizeZ; }

	virtual glm::uvec3 GetSizeXYZ() const override {
		return glm::uvec3(mSizeX, mSizeY, mSizeZ);
	}

private:
	uint32_t mSizeX;
	uint32_t mSizeY;
	uint32_t mSizeZ;
};

class RHITextureCube : public RHITexture
{
public:
	RHITextureCube(uint32_t InSize, uint32_t InNumMips, EPixelFormat InFormat, uint32_t InFlags)
		: RHITexture(InNumMips, 1, InFormat, InFlags)
		, mSize(InSize)
	{}

	RHITextureCube* GetTextureCube() override final { return this; }

	/** @return The width and height of each face of the cubemap. */
	uint32_t GetSize() const { return mSize; }

	virtual glm::uvec3 GetSizeXYZ() const final override {
		return glm::uvec3(mSize, mSize, 1);
	}

private:
	uint32_t mSize;
};

class RHITextureReference : public RHITexture
{
public:
	RHITextureReference()
		: RHITexture(0, 0, PF_Unknown, 0)
	{}

	RHITextureReference* GetTextureReference() override { return this; }
	RHITexture* GetReferencedTexture() { return mReferencedTexture.GetReference(); }

	void SetReferencedTexture(RHITexture* InTexture)
	{
		mReferencedTexture = InTexture;
	}

	virtual glm::uvec3 GetSizeXYZ() const final override
	{
		if (mReferencedTexture) {
			return mReferencedTexture->GetSizeXYZ();
		}
		return glm::uvec3(0, 0, 0);
	}

private:
	TRefCountPtr<RHITexture> mReferencedTexture;
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

// Shader Bindings
typedef std::vector<struct VertexElement> VertexDeclarationElementList;
class RHIVertexDeclaration : public RHIResource
{
public:
	virtual bool GetInitializer(VertexDeclarationElementList& Init) { return false; }
};

class RHIBoundShaderState : public RHIResource {};

/// Shaders
// this is using DirectX naming convention
class RHIShader : public RHIResource {};

class RHIVertexShader : public RHIShader {};
class RHIHullShader : public RHIShader {};
class RHIDomainShader : public RHIShader {};
class RHIPixelShader : public RHIShader {};
class RHIGeometryShader : public RHIShader {};

/// Pipelines
class RHIGraphicsPipelineState;
class RHIGraphicsPipelineStateFallBack;

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
	RHITexture* RenderTarget;

	explicit RHIRenderPassInfo(RHITexture* InRenderTarget) {
		RenderTarget = InRenderTarget;
	}

	explicit RHIRenderPassInfo() = default;
};

#endif // ENGINE_RHI_RHIRESOURCE_H
