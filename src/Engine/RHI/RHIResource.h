
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
