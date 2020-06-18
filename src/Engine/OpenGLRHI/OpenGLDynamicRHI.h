
#ifndef ENGINE_OPENGLRHI_OPENGLDYNAMICRHI_H
#define ENGINE_OPENGLRHI_OPENGLDYNAMICRHI_H

#include "../RHI/DynamicRHI.h"
#include "OpenGLState.h"
#include "OpenGLSpecs.h"

// used 
#define RHI_THREAD_BEGIN_COMMAND() auto OpenGLCommand = [&]() {
#define RHI_THREAD_END_COMMAND() };\
	return OpenGLCommand();
#define RHI_THREAD_END_COMMAND_RETURN(x) };\
	return OpenGLCommand();

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

	// resources
	RHIVertexDeclarationRef RHICreateVertexDeclaration(const VertexDeclarationElementList& Elements) override final;

	RHITexture2DRef RHICreateTexture2D(uint32_t SizeX, uint32_t SizeY, uint8_t Format, uint32_t NumMips, uint32_t NumSamples, uint32_t Flags) override final;
	RHITexture2DRef RHICreateTexture2DFromResource(EPixelFormat Format, uint32_t SizeX, uint32_t SizeY, uint32_t NumMips, uint32_t NumSamples, uint32_t NumSamplesTileMem, uint32_t Flags, GLuint Resource, uint32_t TexCreateFlags) override final;

	// shaders
	RHIVertexShaderRef RHICreateVertexShader(const std::vector<uint8_t>& Code) override final;
	RHIPixelShaderRef RHICreatePixelShader(const std::vector<uint8_t>& Code) override final;

	// states
	RHIBoundShaderStateRef RHICreateBoundShaderState(
		RHIVertexDeclaration* VertexDeclarationRHI,
		RHIVertexShader* VertexShaderRHI,
		RHIPixelShader* PixelShaderRHI) override final;

	// buffers
	RHIVertexBufferRef RHICreateVertexBuffer(size_t VerticesCount, size_t VertexSize, RHIHardwareBufferUsage Usage) override final;
	void* RHILockVertexBuffer(RHIVertexBuffer* VertexBufferRHI, size_t MemberOffset, size_t Size, RHIResourceLockMode LockMode) override final;
	void RHIUnlockVertexBuffer(RHIVertexBuffer* VertexBufferRHI) override final;
	void RHICopyVertexBuffer(RHIVertexBuffer* SourceBufferRHI, RHIVertexBuffer* DestBufferRHI) override final;

	RHIIndexBufferRef RHICreateIndexBuffer(size_t IndicesCount, RHIIndexBufferType Type, RHIHardwareBufferUsage Usage) override final;
	void* RHILockIndexBuffer(RHIIndexBuffer* IndexBufferRHI, size_t MemberOffset, size_t Size, RHIResourceLockMode LockMode) override final;
	void RHIUnlockIndexBuffer(RHIIndexBuffer* IndexBufferRHI) override final;

	RHIUniformBufferRef RHICreateUniformBuffer(const void* Contents, const RHIUniformBufferLayout& Layout, EUniformBufferUsage Usage, EUniformBufferValidation Validation) override final;
	void RHIUpdateUniformBuffer(RHIUniformBuffer* UniformBufferRHI, const void* Contents) override final;

	// shader params
	void RHISetShaderUniformBuffer(RHIVertexShader* VertexShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI) override final;
	void RHISetShaderUniformBuffer(RHIHullShader* HullShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI) override final;
	void RHISetShaderUniformBuffer(RHIDomainShader* DomainShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI) override final;
	void RHISetShaderUniformBuffer(RHIGeometryShader* GeometryShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI) override final;
	void RHISetShaderUniformBuffer(RHIPixelShader* PixelShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI) override final;

	void RHISetShaderParameter(RHIVertexShader* VertexShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue) override final;
	void RHISetShaderParameter(RHIHullShader* HullShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue) override final;
	void RHISetShaderParameter(RHIDomainShader* DomainShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue) override final;
	void RHISetShaderParameter(RHIGeometryShader* GeometryShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue) override final;
	void RHISetShaderParameter(RHIPixelShader* PixelShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue) override final;

	// drawing
	void RHIDrawPrimitive(uint32_t BaseVertexIndex, uint32_t NumPrimitives, uint32_t NumInstances) override final;
	void RHIDrawIndexedPrimitive(RHIIndexBuffer* IndexBufferRHI, uint32_t BaseVertexIndex, uint32_t FirstInstance, uint32_t NumVertices, uint32_t StartIndex, uint32_t NumPrimitives, uint32_t NumInstances) override final;

	void RHISetStreamSource(uint32_t StreamIndex, RHIVertexBuffer* VertexBufferRHI, uint32_t MemberOffset) override final;
	void RHISetViewport(glm::uvec2 Minimum, glm::uvec2 Maximum) override final;
	void RHISetDepth(float MinimumZ, float MaximumZ) override final;
	void RHISetScissorRect(bool Enable, glm::uvec2 Minimum, glm::uvec2 Maximum) override final;
	void RHISetBoundShaderState(RHIBoundShaderState* BoundShaderStateRHI) override final;
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
	void CachedBindUniformBuffer(OpenGLContextState& ContextState, GLuint Buffer);

	OpenGLContextState& GetContextStateForCurrentContext();

private:
	inline uint32_t FindMaxMipmapLevel(uint32_t Size) {
		uint32_t MipCount = 1;
		while (Size >>= 1) {
			MipCount++;
		}
		return MipCount;
	}

	inline uint32_t FindMaxMipmapLevel(uint32_t Width, uint32_t Height) {
		return FindMaxMipmapLevel((Width > Height) ? Width : Height);
	}

	inline uint32_t FindMaxMipmapLevel(uint32_t Width, uint32_t Height, uint32_t Depth) {
		return FindMaxMipmapLevel((Width > Height) ? Width : Height, Depth);
	}

	RHITexture* CreateOpenGLRHITextureOnly(const uint32_t SizeX, const uint32_t SizeY, const bool bCubeTexture, const bool bArrayTexture, const bool bIsExternal, uint8_t& Format, uint32_t& NumMips, uint32_t& NumSamples, const uint32_t ArraySize, uint32_t& Flags);

	RHITexture* CreateOpenGLTexture(uint32_t SizeX, uint32_t SizeY, bool bCubeTexture, bool bArrayTexture, bool bIsExternal, uint8_t Format, uint32_t NumMips, uint32_t NumSamples, uint32_t ArraySize, uint32_t Flags);

	void BindPendingFramebuffer(OpenGLContextState& ContextState);
	void SetPendingBlendStateForActiveRenderTargets(OpenGLContextState& ContextState);
	void UpdateViewportInOpenGLContext(OpenGLContextState& ContextState);
	void UpdateScissorRectInOpenGLContext(OpenGLContextState& ContextState);
	void UpdateRasterizerStateInOpenGLContext(OpenGLContextState& ContextState);
	void BindPendingShaderState(OpenGLContextState& ContextState);
	void CommitNonComputeShaderConstants(OpenGLContextState& ContextState);
	void BindUniformBufferBase(OpenGLContextState& ContextState, int32_t NumUniformBuffers, RHIUniformBufferRef* BoundUniformBuffers, uint32_t FirstUniformBuffer, bool ForceUpdate);

	void SetupTexturesForDraw(OpenGLContextState& ContextState);
	void SetupTexturesForDraw(OpenGLContextState& ContextState, OpenGLBoundShaderState* BoundShaderState, int32_t NumTextures);

	void InitializeStateResources();

	friend class ExampleScene;

	OpenGLRHIState mPendingState;
	OpenGLContextState mInvalidContextState;
	OpenGLContextState mSharedContextState;
	OpenGLContextState mRenderingContextState;

	bool mVsync = false;
	uint32_t mVertexArrayObject = 0;
};

extern void OpenGLDynamicRHICleanVertexBuffer();

#endif // ENGINE_OPENGLRHI_OPENGLDYNAMICRHI_H
