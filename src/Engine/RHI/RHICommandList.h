
#ifndef ENGINE_RHI_RHICOMMANDLIST_H
#define ENGINE_RHI_RHICOMMANDLIST_H

#include "RHIResource.h"
#include "DynamicRHI.h"

// adapted from unreal engine
// for now all it does is actually simulating the dynamic RHI
// but later on depending on the API it can actually represent
// a real command queue

class RHICommandListBase
{
};

class RHICommandList : public RHICommandListBase
{
public:

	virtual inline void BeginScene() {
		gDynamicRHI->RHIBeginScene();
	}

	virtual inline void EndScene() {
		gDynamicRHI->RHIEndScene();
	}
	virtual inline void BeginFrame() {
		gDynamicRHI->RHIBeginFrame();
	}

	virtual inline void EndFrame() {
		gDynamicRHI->RHIEndFrame();
	}

	virtual inline void BeginRenderPass(RHIRenderPassInfo& Info) {
		gDynamicRHI->RHIBeginRenderPass(Info);
	}

	virtual inline void EndRenderPass() {
		gDynamicRHI->RHIEndRenderPass();
	}

	virtual inline RHIVertexBuffer_ptr CreateVertexBuffer(size_t VerticesCount, size_t VertexSize, RHIHardwareBufferUsage Usage) {
		return gDynamicRHI->RHICreateVertexBuffer(VerticesCount, VertexSize, Usage);
	}
	virtual inline void* LockVertexBuffer(RHIVertexBuffer_ptr VertexBufferRHI, size_t Offset, size_t Size, RHIResourceLockMode LockMode) {
		return gDynamicRHI->RHILockVertexBuffer(VertexBufferRHI, Offset, Size, LockMode);
	}
	virtual inline void UnlockVertexBuffer(RHIVertexBuffer_ptr VertexBufferRHI) {
		gDynamicRHI->RHIUnlockVertexBuffer(VertexBufferRHI);
	}
	virtual inline void CopyVertexBuffer(RHIVertexBuffer_ptr SourceBufferRHI, RHIVertexBuffer_ptr DestBufferRHI) {
		gDynamicRHI->RHICopyVertexBuffer(SourceBufferRHI, DestBufferRHI);
	}

	virtual inline RHIIndexBuffer_ptr CreateIndexBuffer(size_t IndicesCount, RHIIndexBufferType Type, RHIHardwareBufferUsage Usage) {
		return gDynamicRHI->RHICreateIndexBuffer(IndicesCount, Type, Usage);
	}
	virtual inline void* LockIndexBuffer(RHIIndexBuffer_ptr IndexBufferRHI, size_t Offset, size_t Size, RHIResourceLockMode LockMode) {
		return gDynamicRHI->RHILockIndexBuffer(IndexBufferRHI, Offset, Size, LockMode);
	}
	virtual inline void UnlockIndexBuffer(RHIIndexBuffer_ptr IndexBufferRHI) {
		gDynamicRHI->RHIUnlockIndexBuffer(IndexBufferRHI);
	}

	virtual inline RHIVertexShader_ptr CreateVertexShader(const std::vector<uint8_t>& Code) {
		return gDynamicRHI->RHICreateVertexShader(Code);
	}
	virtual inline RHIPixelShader_ptr CreatePixelShader(const std::vector<uint8_t>& Code) {
		return gDynamicRHI->RHICreatePixelShader(Code);
	}

	virtual inline RHIBoundShaderState_ptr CreateBoundShaderState( RHIVertexDeclaration_ptr VertexDeclarationRHI, RHIVertexShader_ptr VertexShaderRHI, RHIPixelShader_ptr PixelShaderRHI) {
		return gDynamicRHI->RHICreateBoundShaderState(VertexDeclarationRHI, VertexShaderRHI, PixelShaderRHI);
	}

	virtual inline void DrawPrimitive(uint32_t BaseVertexIndex, uint32_t NumPrimitives, uint32_t NumInstances) {
		gDynamicRHI->RHIDrawPrimitive(BaseVertexIndex, NumPrimitives, NumInstances);
	}
	virtual inline void DrawIndexedPrimitive(RHIIndexBuffer_ptr IndexBufferRHI, uint32_t BaseVertexIndex, uint32_t FirstInstance, uint32_t NumVertices, uint32_t StartIndex, uint32_t NumPrimitives, uint32_t NumInstances) {
		gDynamicRHI->RHIDrawIndexedPrimitive(IndexBufferRHI, BaseVertexIndex, FirstInstance, NumVertices, StartIndex, NumPrimitives, NumInstances);
	}

	// TODO: these should only be called from the immediate context
	virtual inline void SetStreamSource(uint32_t StreamIndex, RHIVertexBuffer_ptr VertexBufferRHI, uint32_t Offset) {
		gDynamicRHI->RHISetStreamSource(StreamIndex, VertexBufferRHI, Offset);
	}
	virtual inline void SetViewport(glm::uvec2 Minimum, glm::uvec2 Maximum) {
		gDynamicRHI->RHISetViewport(Minimum, Maximum);
	}
	virtual inline void SetDepth(float MinimumZ, float MaximumZ) {
		gDynamicRHI->RHISetDepth(MinimumZ, MaximumZ);
	}
	virtual inline void SetScissorRect(bool Enable, glm::uvec2 Minimum, glm::uvec2 Maximum) {
		gDynamicRHI->RHISetScissorRect(Enable, Minimum, Maximum);
	}
	virtual inline void SetBoundShaderState(RHIBoundShaderState_ptr BoundShaderStateRHI) {
		gDynamicRHI->RHISetBoundShaderState(BoundShaderStateRHI);
	}
private:
};

class RHICommandListImmediate : public RHICommandList
{
public:
private:
};

class RHICommandListExecutor
{
public:
	static RHICommandListImmediate& GetImmediateCommandList();

private:
	RHICommandListImmediate ImmediateCommandList;
};

extern RHICommandListExecutor gRHICommandList;

#endif // ENGINE_RHI_RHICOMMANDLIST_H
