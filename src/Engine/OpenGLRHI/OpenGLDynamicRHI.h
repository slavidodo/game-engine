
#ifndef ENGINE_OPENGLRHI_OPENGLDYNAMICRHI_H
#define ENGINE_OPENGLRHI_OPENGLDYNAMICRHI_H

#include "../RHI/DynamicRHI.h"
#include "OpenGLState.h"

class OpenGLDynamicRHI : public DynamicRHI
{
public:
	// TODO: Remove any context-related information
	OpenGLDynamicRHI() = default;

	bool Init() override final;
	void Terminate() override final;

	bool GetVsync() const { return mVsync; }
	void SetVsync(bool value) {
		mVsync = value;
		glfwSwapInterval(mVsync);
	}

	void ClearScreen(bool color, bool depth) override final;
	void ClearColor(glm::fvec4 color) override final;

	RHIVertexDeclaration_ptr RHICreateVertexDeclaration(const VertexDeclarationElementList& Elements) override final;

	// shaders
	RHIVertexShader_ptr RHICreateVertexShader(const std::vector<uint8_t>& Code) override final;
	RHIPixelShader_ptr RHICreatePixelShader(const std::vector<uint8_t>& Code) override final;

	// states
	RHIBoundShaderState_ptr RHICreateBoundShaderState(
		RHIVertexDeclaration_ptr VertexDeclarationRHI,
		RHIVertexShader_ptr VertexShaderRHI,
		RHIPixelShader_ptr PixelShaderRHI) override final;

	// buffers
	RHIVertexBuffer_ptr RHICreateVertexBuffer(size_t VerticesCount, size_t VertexSize, RHIHardwareBufferUsage Usage) override final;
	void* RHILockVertexBuffer(RHIVertexBuffer_ptr VertexBufferRHI, size_t Offset, size_t Size, RHIResourceLockMode LockMode) override final;
	void RHIUnlockVertexBuffer(RHIVertexBuffer_ptr VertexBufferRHI) override final;
	void RHICopyVertexBuffer(RHIVertexBuffer_ptr SourceBufferRHI, RHIVertexBuffer_ptr DestBufferRHI) override final;

	RHIIndexBuffer_ptr RHICreateIndexBuffer(size_t IndicesCount, RHIIndexBufferType Type, RHIHardwareBufferUsage Usage) override final;
	void* RHILockIndexBuffer(RHIIndexBuffer_ptr IndexBufferRHI, size_t Offset, size_t Size, RHIResourceLockMode LockMode) override final;
	void RHIUnlockIndexBuffer(RHIIndexBuffer_ptr IndexBufferRHI) override final;

	// drawing
	void RHIDrawPrimitive(uint32_t BaseVertexIndex, uint32_t NumPrimitives, uint32_t NumInstances) override final;
	void RHIDrawIndexedPrimitive(RHIIndexBuffer_ptr IndexBufferRHI, uint32_t BaseVertexIndex, uint32_t FirstInstance, uint32_t NumVertices, uint32_t StartIndex, uint32_t NumPrimitives, uint32_t NumInstances) override final;

	void RHISetStreamSource(uint32_t StreamIndex, RHIVertexBuffer_ptr VertexBufferRHI, uint32_t Offset) override final;
	void RHISetViewport(glm::uvec2 Minimum, glm::uvec2 Maximum) override final;
	void RHISetDepth(float MinimumZ, float MaximumZ) override final;
	void RHISetScissorRect(bool Enable, glm::uvec2 Minimum, glm::uvec2 Maximum) override final;
	void RHISetBoundShaderState(RHIBoundShaderState_ptr BoundShaderStateRHI) override final;
	void RHISetGraphicsPipelineState(RHIGraphicsPipelineState GraphicsState) override final {
		// TODO: extract bound shader state information
		// this includes all shader stages & vertex declaration
	}

	void RHIBeginScene() override final;
	void RHIEndScene() override final;
	void RHIBeginFrame() override final;
	void RHIEndFrame() override final;

	void RHIBeginRenderPass(RHIRenderPassInfo& Info) override final;
	void RHIEndRenderPass() override final;

	// opengl-only
	GLenum GetGLUsageForHardwareUsage(RHIHardwareBufferUsage hbUsage);
	void SetupVertexArrays(OpenGLContextState& ContextState, uint32_t BaseVertexIndex, OpenGLStream* Streams, uint32_t NumStreams, uint32_t MaxVertices);
	void EnableVertexElementCached(OpenGLContextState& ContextState, GLuint AttributeIndex,
								   const OpenGLVertexElement& VertexElement, GLsizei Stride,
								   void* Pointer, GLuint Buffer);
	void CachedBindArrayBuffer(OpenGLContextState& ContextState, GLuint Buffer);
	void CachedBindElementArrayBuffer(OpenGLContextState& ContextState, GLuint Buffer);

private:
	OpenGLContextState& GetContextStateForCurrentContext();
	void BindPendingFramebuffer(OpenGLContextState& ContextState);
	void UpdateViewportInOpenGLContext(OpenGLContextState& ContextState);
	void UpdateScissorRectInOpenGLContext(OpenGLContextState& ContextState);
	void UpdateRasterizerStateInOpenGLContext(OpenGLContextState& ContextState);
	void BindPendingShaderState(OpenGLContextState& ContextState);

	friend class ExampleScene;

	OpenGLRHIState mPendingState;
	OpenGLContextState mRenderingContextState;

	bool mVsync = false;
	uint32_t mVertexArrayObject = 0;
};

#endif // ENGINE_OPENGLRHI_OPENGLDYNAMICRHI_H
