
#ifndef ENGINE_RHI_DYNAMICRHI_H
#define ENGINE_RHI_DYNAMICRHI_H

#include "../Core/Camera.h"
#include "../Core/Material.h"
#include "../Core/StaticMesh.h"
#include "../Core/Transform.h"
#include "RHIResource.h"
#include "RHIDefinitions.h"

class DynamicRHI
{
public:
	DynamicRHI() = default;

	virtual bool Init() = 0;
	virtual void Terminate() = 0;

	virtual void SetVsync(bool value) = 0;
	virtual bool GetVsync() const = 0;

	virtual void ClearScreen(bool color, bool depth) = 0;
	virtual void ClearColor(glm::fvec4 color) = 0;

	// drawing functionality
	virtual void RHIDrawPrimitive(uint32_t BaseVertexIndex, uint32_t NumPrimitives, uint32_t NumInstances) = 0;
	virtual void RHIDrawIndexedPrimitive(RHIIndexBuffer* IndexBufferRHI, uint32_t BaseVertexIndex, uint32_t FirstInstance, uint32_t NumVertices, uint32_t StartIndex, uint32_t NumPrimitives, uint32_t NumInstances) = 0;

	virtual void RHISetStreamSource(uint32_t StreamIndex, RHIVertexBuffer* VertexBufferRHI, uint32_t MemberOffset) = 0;
	virtual void RHISetViewport(glm::uvec2 Minimum, glm::uvec2 Maximum) = 0;
	virtual void RHISetDepth(float MinimumZ, float MaximumZ) = 0;
	virtual void RHISetScissorRect(bool Enable, glm::uvec2 Minimum, glm::uvec2 Maximum) = 0;
	virtual void RHISetBoundShaderState(RHIBoundShaderState* BoundShaderStateRHI) = 0;
	virtual void RHISetGraphicsPipelineState(RHIGraphicsPipelineState GraphicsState) = 0;

	// resources
	virtual RHIVertexDeclarationRef RHICreateVertexDeclaration(const VertexDeclarationElementList& Elements) = 0;

	virtual RHITexture2DRef RHICreateTexture2D(uint32_t SizeX, uint32_t SizeY, uint8_t Format, uint32_t NumMips, uint32_t NumSamples, uint32_t Flags) = 0;
	virtual RHITexture2DRef RHICreateTexture2DFromResource(EPixelFormat Format, uint32_t SizeX, uint32_t SizeY, uint32_t NumMips, uint32_t NumSamples, uint32_t NumSamplesTileMem, uint32_t Flags, GLuint Resource, uint32_t TexCreateFlags) = 0;

	// shaders
	virtual RHIVertexShaderRef RHICreateVertexShader(const std::vector<uint8_t>& Code) = 0;
	virtual RHIPixelShaderRef RHICreatePixelShader(const std::vector<uint8_t>& Code) = 0;

	// shader params
	virtual void RHISetShaderUniformBuffer(RHIVertexShader* VertexShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI) = 0;
	virtual void RHISetShaderUniformBuffer(RHIHullShader* HullShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI) = 0;
	virtual void RHISetShaderUniformBuffer(RHIDomainShader* DomainShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI) = 0;
	virtual void RHISetShaderUniformBuffer(RHIGeometryShader* GeometryShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI) = 0;
	virtual void RHISetShaderUniformBuffer(RHIPixelShader* PixelShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI) = 0;

	virtual void RHISetShaderParameter(RHIVertexShader* VertexShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue) = 0;
	virtual void RHISetShaderParameter(RHIHullShader* HullShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue) = 0;
	virtual void RHISetShaderParameter(RHIDomainShader* DomainShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue) = 0;
	virtual void RHISetShaderParameter(RHIGeometryShader* GeometryShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue) = 0;
	virtual void RHISetShaderParameter(RHIPixelShader* PixelShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue) = 0;

	// states
	virtual RHIBoundShaderStateRef RHICreateBoundShaderState(
		RHIVertexDeclaration* VertexDeclarationRHI,
		RHIVertexShader* VertexShaderRHI,
		RHIPixelShader* PixelShaderRHI) = 0;

	// hardware buffers
	virtual RHIVertexBufferRef RHICreateVertexBuffer(size_t VerticesCount, size_t VertexSize, RHIHardwareBufferUsage Usage) = 0;
	virtual void* RHILockVertexBuffer(RHIVertexBuffer* VertexBufferRHI, size_t MemberOffset, size_t Size, RHIResourceLockMode LockMode) = 0;
	virtual void RHIUnlockVertexBuffer(RHIVertexBuffer* VertexBufferRHI) = 0;
	virtual void RHICopyVertexBuffer(RHIVertexBuffer* SourceBufferRHI, RHIVertexBuffer* DestBufferRHI) = 0;

	virtual RHIIndexBufferRef RHICreateIndexBuffer(size_t IndicesCount, RHIIndexBufferType Type, RHIHardwareBufferUsage Usage) = 0;
	virtual void* RHILockIndexBuffer(RHIIndexBuffer* IndexBufferRHI, size_t MemberOffset, size_t Size, RHIResourceLockMode LockMode) = 0;
	virtual void RHIUnlockIndexBuffer(RHIIndexBuffer* IndexBufferRHI) = 0;

	virtual RHIUniformBufferRef RHICreateUniformBuffer(const void* Contents, const RHIUniformBufferLayout& Layout, EUniformBufferUsage Usage, EUniformBufferValidation Validation) = 0;
	virtual void RHIUpdateUniformBuffer(RHIUniformBuffer* UniformBufferRHI, const void* Contents) = 0;

	// scenes
	virtual void RHIBeginScene() = 0;
	virtual void RHIEndScene() = 0;
	virtual void RHIBeginFrame() = 0;
	virtual void RHIEndFrame() = 0;

	// utilize profiling, swap chain, ...
	virtual void RHIBeginRenderPass(RHIRenderPassInfo& Info) = 0;
	virtual void RHIEndRenderPass() = 0;
};

extern DynamicRHI* gDynamicRHI;

#endif // ENGINE_RHI_DYNAMICRHI_H
