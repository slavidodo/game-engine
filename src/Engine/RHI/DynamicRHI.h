
#ifndef ENGINE_RHI_DYNAMICRHI_H
#define ENGINE_RHI_DYNAMICRHI_H

#include "../Core/Camera.h"
#include "../Core/Material.h"
#include "../Core/StaticMesh.h"
#include "../Core/Transform.h"
#include "RHIResource.h"

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
	virtual void RHIDrawIndexedPrimitive(RHIIndexBuffer_ptr IndexBufferRHI, uint32_t BaseVertexIndex, uint32_t FirstInstance, uint32_t NumVertices, uint32_t StartIndex, uint32_t NumPrimitives, uint32_t NumInstances) = 0;

	virtual void RHISetStreamSource(uint32_t StreamIndex, RHIVertexBuffer_ptr VertexBufferRHI, uint32_t Offset) = 0;
	virtual void RHISetViewport(glm::uvec2 Minimum, glm::uvec2 Maximum) = 0;
	virtual void RHISetDepth(float MinimumZ, float MaximumZ) = 0;
	virtual void RHISetScissorRect(bool Enable, glm::uvec2 Minimum, glm::uvec2 Maximum) = 0;
	virtual void RHISetBoundShaderState(RHIBoundShaderState_ptr BoundShaderStateRHI) = 0;
	virtual void RHISetGraphicsPipelineState(RHIGraphicsPipelineState GraphicsState) = 0;

	// resources
	virtual RHIVertexDeclaration_ptr RHICreateVertexDeclaration(const VertexDeclarationElementList& Elements) = 0;

	// shaders
	virtual RHIVertexShader_ptr RHICreateVertexShader(const std::vector<uint8_t>& Code) = 0;
	virtual RHIPixelShader_ptr RHICreatePixelShader(const std::vector<uint8_t>& Code) = 0;

	// states
	virtual RHIBoundShaderState_ptr RHICreateBoundShaderState(
		RHIVertexDeclaration_ptr VertexDeclarationRHI,
		RHIVertexShader_ptr VertexShaderRHI,
		RHIPixelShader_ptr PixelShaderRHI) = 0;

	// hardware buffers
	virtual RHIVertexBuffer_ptr RHICreateVertexBuffer(size_t VerticesCount, size_t VertexSize, RHIHardwareBufferUsage Usage) = 0;
	virtual void* RHILockVertexBuffer(RHIVertexBuffer_ptr VertexBufferRHI, size_t Offset, size_t Size, RHIResourceLockMode LockMode) = 0;
	virtual void RHIUnlockVertexBuffer(RHIVertexBuffer_ptr VertexBufferRHI) = 0;
	virtual void RHICopyVertexBuffer(RHIVertexBuffer_ptr SourceBufferRHI, RHIVertexBuffer_ptr DestBufferRHI) = 0;

	virtual RHIIndexBuffer_ptr RHICreateIndexBuffer(size_t IndicesCount, RHIIndexBufferType Type, RHIHardwareBufferUsage Usage) = 0;
	virtual void* RHILockIndexBuffer(RHIIndexBuffer_ptr IndexBufferRHI, size_t Offset, size_t Size, RHIResourceLockMode LockMode) = 0;
	virtual void RHIUnlockIndexBuffer(RHIIndexBuffer_ptr IndexBufferRHI) = 0;

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
