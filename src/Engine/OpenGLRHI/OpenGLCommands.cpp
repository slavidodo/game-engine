
#include "pch.h"

#include "OpenGLDynamicRHI.h"

// Vertex State.
void OpenGLDynamicRHI::RHISetStreamSource(uint32_t StreamIndex, RHIVertexBuffer* VertexBufferRHI, uint32_t MemberOffset)
{
	OpenGLVertexBuffer* VertexBuffer = StaticResourceCast(VertexBufferRHI);

	mPendingState.Streams[StreamIndex].VertexBuffer = VertexBuffer;
	mPendingState.Streams[StreamIndex].Stride = 0;
	mPendingState.Streams[StreamIndex].MemberOffset = MemberOffset;
}

// Rasterizer State.
void OpenGLDynamicRHI::UpdateRasterizerStateInOpenGLContext(OpenGLContextState& ContextState)
{
	if (ContextState.RasterizerState.FillMode != mPendingState.RasterizerState.FillMode) {
		glPolygonMode(GL_FRONT_AND_BACK, mPendingState.RasterizerState.FillMode);
		ContextState.RasterizerState.FillMode = mPendingState.RasterizerState.FillMode;
	}

	if (ContextState.RasterizerState.CullMode != mPendingState.RasterizerState.CullMode) {
		if (mPendingState.RasterizerState.CullMode != GL_NONE) {
			if (ContextState.RasterizerState.CullMode == GL_NONE) {
				glEnable(GL_CULL_FACE);
			}

			glCullFace(mPendingState.RasterizerState.CullMode);
		} else {
			glDisable(GL_CULL_FACE);
		}

		ContextState.RasterizerState.CullMode = mPendingState.RasterizerState.CullMode;
	}
}

void OpenGLDynamicRHI::RHISetViewport(glm::uvec2 Minimum, glm::uvec2 Maximum)
{
	mPendingState.Viewport = IntRect(Minimum, Maximum);

	RHISetScissorRect(false, glm::uvec2(0), glm::uvec2(0));
}

void OpenGLDynamicRHI::UpdateViewportInOpenGLContext(OpenGLContextState& ContextState)
{
	if (ContextState.Viewport != mPendingState.Viewport) {
		IntRect::TPoint position = mPendingState.Viewport.GetMinimum();
		IntRect::TPoint size = mPendingState.Viewport.GetSize();

		glViewport(position.x, position.y, size.x, size.y);
		ContextState.Viewport = mPendingState.Viewport;
	}
}

void OpenGLDynamicRHI::RHISetDepth(float MinimumZ, float MaximumZ)
{
	mPendingState.DepthMinZ = MinimumZ;
	mPendingState.DepthMinZ = MaximumZ;
}

void OpenGLDynamicRHI::RHISetScissorRect(bool Enable, glm::uvec2 Mininum, glm::uvec2 Maximum)
{
	mPendingState.ScissorEnabled = Enable;
	mPendingState.Scissor = IntRect(Mininum, Maximum);
}

void OpenGLDynamicRHI::UpdateScissorRectInOpenGLContext(OpenGLContextState& ContextState)
{
	if (ContextState.ScissorEnabled != mPendingState.ScissorEnabled) {
		if (mPendingState.ScissorEnabled) {
			glEnable(GL_SCISSOR_TEST);
		} else {
			glDisable(GL_SCISSOR_TEST);
		}
		ContextState.ScissorEnabled = mPendingState.ScissorEnabled;
	}

	if (mPendingState.ScissorEnabled && ContextState.Scissor != mPendingState.Scissor) {
		IntRect::TPoint position = mPendingState.Scissor.GetMinimum();
		IntRect::TPoint size = mPendingState.Scissor.GetSize();

		glScissor(position.x, position.y, size.x, size.y);
		ContextState.Scissor = mPendingState.Scissor;
	}
}

// Bound Shader State.
void OpenGLDynamicRHI::RHISetBoundShaderState(RHIBoundShaderState* BoundShaderStateRHI)
{
	OpenGLBoundShaderState* BoundShaderState = StaticResourceCast(BoundShaderStateRHI);
	mPendingState.BoundShaderState = BoundShaderState;
}

void OpenGLDynamicRHI::RHISetShaderUniformBuffer(RHIVertexShader* VertexShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI)
{
	mPendingState.BoundUniformBuffers[SF_Vertex][BufferIndex] = BufferRHI;
	mPendingState.DirtyUniformBuffers[SF_Vertex] |= 1 << BufferIndex;
	mPendingState.AnyDirtyUniformBuffers = true;
}
void OpenGLDynamicRHI::RHISetShaderUniformBuffer(RHIHullShader* HullShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI)
{
	mPendingState.BoundUniformBuffers[SF_Hull][BufferIndex] = BufferRHI;
	mPendingState.DirtyUniformBuffers[SF_Hull] |= 1 << BufferIndex;
	mPendingState.AnyDirtyUniformBuffers = true;
}
void OpenGLDynamicRHI::RHISetShaderUniformBuffer(RHIDomainShader* DomainShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI)
{
	mPendingState.BoundUniformBuffers[SF_Domain][BufferIndex] = BufferRHI;
	mPendingState.DirtyUniformBuffers[SF_Domain] |= 1 << BufferIndex;
	mPendingState.AnyDirtyUniformBuffers = true;
}
void OpenGLDynamicRHI::RHISetShaderUniformBuffer(RHIGeometryShader* GeometryShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI)
{
	mPendingState.BoundUniformBuffers[SF_Geometry][BufferIndex] = BufferRHI;
	mPendingState.DirtyUniformBuffers[SF_Geometry] |= 1 << BufferIndex;
	mPendingState.AnyDirtyUniformBuffers = true;
}
void OpenGLDynamicRHI::RHISetShaderUniformBuffer(RHIPixelShader* PixelShaderRHI, uint32_t BufferIndex, RHIUniformBuffer* BufferRHI)
{
	mPendingState.BoundUniformBuffers[SF_Pixel][BufferIndex] = BufferRHI;
	mPendingState.DirtyUniformBuffers[SF_Pixel] |= 1 << BufferIndex;
	mPendingState.AnyDirtyUniformBuffers = true;
}

void OpenGLDynamicRHI::RHISetShaderParameter(RHIVertexShader* VertexShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue)
{
	mPendingState.ShaderParameters[ShaderStage::SHADER_STAGE_VERTEX].Set(BufferIndex, BaseIndex, NumBytes, NewValue);
	mPendingState.LinkedProgramAndDirtyFlag = nullptr;
}
void OpenGLDynamicRHI::RHISetShaderParameter(RHIHullShader* HullShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue)
{
	mPendingState.ShaderParameters[ShaderStage::SHADER_STAGE_HULL].Set(BufferIndex, BaseIndex, NumBytes, NewValue);
	mPendingState.LinkedProgramAndDirtyFlag = nullptr;
}
void OpenGLDynamicRHI::RHISetShaderParameter(RHIDomainShader* DomainShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue)
{
	mPendingState.ShaderParameters[ShaderStage::SHADER_STAGE_DOMAIN].Set(BufferIndex, BaseIndex, NumBytes, NewValue);
	mPendingState.LinkedProgramAndDirtyFlag = nullptr;
}
void OpenGLDynamicRHI::RHISetShaderParameter(RHIGeometryShader* GeometryShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue)
{
	mPendingState.ShaderParameters[ShaderStage::SHADER_STAGE_GEOMETRY].Set(BufferIndex, BaseIndex, NumBytes, NewValue);
	mPendingState.LinkedProgramAndDirtyFlag = nullptr;
}
void OpenGLDynamicRHI::RHISetShaderParameter(RHIPixelShader* PixelShaderRHI, uint32_t BufferIndex, uint32_t BaseIndex, uint32_t NumBytes, const void* NewValue)
{
	mPendingState.ShaderParameters[ShaderStage::SHADER_STAGE_PIXEL].Set(BufferIndex, BaseIndex, NumBytes, NewValue);
	mPendingState.LinkedProgramAndDirtyFlag = nullptr;
}

void OpenGLDynamicRHI::SetPendingBlendStateForActiveRenderTargets(OpenGLContextState& ContextState)
{
	// TODO blend state
	// TODO 
}

// Primitive Drawing
void OpenGLDynamicRHI::EnableVertexElementCached(
	OpenGLContextState& ContextState,
	GLuint AttributeIndex,
	const OpenGLVertexElement& VertexElement,
	GLsizei Stride,
	void* Pointer,
	GLuint Buffer)
{
	if (!ContextState.GetVertexAttrEnabled(AttributeIndex)) {
		ContextState.SetVertexAttrEnabled(AttributeIndex, true);
		glEnableVertexAttribArray(AttributeIndex);
	}
	OpenGLCachedAttr& Attr = ContextState.VertexAttrs[AttributeIndex];

	bool hasAnyDifference = //bitwise ors to get rid of the branches
		(Attr.Pointer != Pointer) |
		(Attr.Buffer != Buffer) |
		(Attr.Size != VertexElement.Size) |
		(Attr.Type != VertexElement.Type) |
		(Attr.Normalized != VertexElement.Normalized) |
		(Attr.Stride != Stride);

	if (hasAnyDifference) {
		CachedBindArrayBuffer(ContextState, Buffer);
		if (!VertexElement.ShouldConvertToFloat) {
			glVertexAttribIPointer(
				AttributeIndex,
				VertexElement.Size,
				VertexElement.Type,
				Stride,
				Pointer
			);
		} else {
			glVertexAttribPointer(
				AttributeIndex,
				VertexElement.Size,
				VertexElement.Type,
				VertexElement.Normalized,
				Stride,
				Pointer
			);
		}

		Attr.Pointer = Pointer;
		Attr.Buffer = Buffer;
		Attr.Size = VertexElement.Size;
		Attr.Type = VertexElement.Type;
		Attr.Normalized = VertexElement.Normalized;
		Attr.Stride = Stride;
	}

	// TODO: instancing
	if (Attr.Divisor != VertexElement.Divisor) {
		glVertexAttribDivisor(AttributeIndex, VertexElement.Divisor);
		Attr.Divisor = VertexElement.Divisor;
	}
}

void OpenGLDynamicRHI::SetupVertexArrays(OpenGLContextState& ContextState, uint32_t BaseVertexIndex, OpenGLStream* Streams, uint32_t NumStreams, uint32_t MaxVertices)
{
	// TODO: if we support VAB, there would be a different path
	if (OpenGLSpecs::SupportsVertexAttribBinding()) {
		// VAB
	}

	uint32_t UsedAttributes = 0;

	OpenGLVertexDeclaration* VertexDeclaration = mPendingState.BoundShaderState->GetVertexDeclaration();
	for (size_t ElementIndex = 0; ElementIndex < VertexDeclaration->VertexElements.size(); ElementIndex++) {
		OpenGLVertexElement& VertexElement = VertexDeclaration->VertexElements[ElementIndex];

		uint8_t AttributeIndex = VertexElement.AttributeIndex;
		// TODO: use shader bindings to check if the shader actually uses this attribute

		if (VertexElement.StreamIndex < NumStreams) {
			OpenGLStream* Stream = &Streams[VertexElement.StreamIndex];

			if (!ContextState.GetVertexAttrEnabled(AttributeIndex)) {
				ContextState.SetVertexAttrEnabled(AttributeIndex, true);
				glEnableVertexAttribArray(AttributeIndex);
			}

			// zero-stride
			EnableVertexElementCached(ContextState,
									  AttributeIndex,
									  VertexElement,
									  (GLsizei)Stream->VertexBuffer->GetVertexSize(),
									  (void*)(uintptr_t)(BaseVertexIndex * Stream->Stride + Stream->MemberOffset + VertexElement.MemberOffset),
									  Stream->VertexBuffer->GetGLResource());

			UsedAttributes |= (1 << AttributeIndex);
		} else {
			if (ContextState.GetVertexAttrEnabled(AttributeIndex)) {
				ContextState.SetVertexAttrEnabled(AttributeIndex, false);
				glDisableVertexAttribArray(AttributeIndex);
			}

			float data[4] = { 0.0f };
			glVertexAttrib4fv(AttributeIndex, data);
		}
	}

	uint32_t NotUsedButEnabledMask = (ContextState.VertexAttrsEnabledBits & ~(UsedAttributes));
	for (GLuint AttribIndex = 0; AttribIndex < NUM_OPENGL_VERTEX_STREAMS && NotUsedButEnabledMask; AttribIndex++) {
		if (NotUsedButEnabledMask & 1) {
			glDisableVertexAttribArray(AttribIndex);
			ContextState.SetVertexAttrEnabled(AttribIndex, false);
		}
		NotUsedButEnabledMask >>= 1;
	}
}

void OpenGLDynamicRHI::CommitNonComputeShaderConstants(OpenGLContextState& ContextState)
{
	if (!OpenGLSpecs::SupportsUniformBuffers()) {
		mPendingState.LinkedProgramAndDirtyFlag = nullptr;
		OpenGLLinkedProgram* LinkedProgram = mPendingState.BoundShaderState->GetLinkedProgram();

		// Vertex Shader
		OpenGLShaderParameterCache& ShaderParams = mPendingState.ShaderParameters[SHADER_STAGE_VERTEX];
		OpenGLVertexShader* VertexShader = mPendingState.BoundShaderState->GetVertexShader();
		ShaderParams.CommitPackedUniformBuffers(LinkedProgram, SHADER_STAGE_VERTEX, mPendingState.BoundUniformBuffers[SF_Vertex], VertexShader->UniformBuffersCopyInfo);
		ShaderParams.CommitPackedGlobals(LinkedProgram, SHADER_STAGE_VERTEX);

		// Pixel Shader
		ShaderParams = mPendingState.ShaderParameters[SHADER_STAGE_PIXEL];
		OpenGLPixelShader* PixelShader = mPendingState.BoundShaderState->GetPixelShader();
		ShaderParams.CommitPackedUniformBuffers(LinkedProgram, SHADER_STAGE_PIXEL, mPendingState.BoundUniformBuffers[SF_Vertex], PixelShader->UniformBuffersCopyInfo);
		ShaderParams.CommitPackedGlobals(LinkedProgram, SHADER_STAGE_PIXEL);

		// TODO(slavi): geometry shader
	} else {

		OpenGLLinkedProgram* LinkedProgram = mPendingState.BoundShaderState->GetLinkedProgram();
		if (LinkedProgram == mPendingState.LinkedProgramAndDirtyFlag) {
			return;
		}

		mPendingState.ShaderParameters[SHADER_STAGE_VERTEX].CommitPackedGlobals(LinkedProgram, SHADER_STAGE_VERTEX);
		mPendingState.ShaderParameters[SHADER_STAGE_PIXEL].CommitPackedGlobals(LinkedProgram, SHADER_STAGE_PIXEL);

		// TODO(slavi): geometry shader

		mPendingState.LinkedProgramAndDirtyFlag = LinkedProgram;
	}
}

void OpenGLDynamicRHI::SetupTexturesForDraw(OpenGLContextState& ContextState)
{
	SetupTexturesForDraw(ContextState, mPendingState.BoundShaderState, OpenGLSpecs::GetMaxCombinedTextureImageUnits());
}

void OpenGLDynamicRHI::SetupTexturesForDraw(OpenGLContextState& ContextState, OpenGLBoundShaderState* BoundShaderState, int32_t NumTextures)
{
	// via gl_ARB_bindless_texture, we pass texture by number instead
	if (OpenGLSpecs::SupportsBindlessTextures()) {
		return;
	}
}

void OpenGLDynamicRHI::RHIDrawPrimitive(uint32_t BaseVertexIndex, uint32_t NumPrimitives, uint32_t NumInstances)
{
	RHI_THREAD_BEGIN_COMMAND();
	OpenGLContextState& ContextState = GetContextStateForCurrentContext();
	BindPendingFramebuffer(ContextState);
	//SetPendingBlendStateForActiveRenderTargets(ContextState);
	UpdateViewportInOpenGLContext(ContextState);
	UpdateScissorRectInOpenGLContext(ContextState);
	UpdateRasterizerStateInOpenGLContext(ContextState);
	//UpdateDepthStencilStateInOpenGLContext(ContextState);
	BindPendingShaderState(ContextState);
	//CommitGraphicsResourceTables();
	SetupTexturesForDraw(ContextState);
	CommitNonComputeShaderConstants(ContextState);
	CachedBindElementArrayBuffer(ContextState, 0);

	uint32_t VertexCount = NumPrimitives * 3;
	SetupVertexArrays(ContextState, BaseVertexIndex, mPendingState.Streams, NUM_OPENGL_VERTEX_STREAMS, VertexCount);

	GLenum DrawMode = GL_TRIANGLES;
	GLsizei NumElements = NumPrimitives * 3;
	if (NumInstances == 1) {
		glDrawArrays(DrawMode, 0, NumElements);
	} else {
		glDrawArraysInstanced(GL_TRIANGLES, 0, NumElements, NumInstances);
	}
	RHI_THREAD_END_COMMAND();
}

void OpenGLDynamicRHI::RHIDrawIndexedPrimitive(RHIIndexBuffer* IndexBufferRHI, uint32_t BaseVertexIndex, uint32_t FirstInstance, uint32_t NumVertices, uint32_t StartIndex, uint32_t NumPrimitives, uint32_t NumInstances)
{
	RHI_THREAD_BEGIN_COMMAND();
	OpenGLIndexBuffer* IndexBuffer = StaticResourceCast(IndexBufferRHI);

	OpenGLContextState& ContextState = GetContextStateForCurrentContext();

	BindPendingFramebuffer(ContextState);
	// TODO: Blending
	SetPendingBlendStateForActiveRenderTargets(ContextState);
	UpdateViewportInOpenGLContext(ContextState);
	UpdateScissorRectInOpenGLContext(ContextState);
	UpdateRasterizerStateInOpenGLContext(ContextState);
	glEnable(GL_DEPTH_TEST); // TODO: Depth Stencil
	BindPendingShaderState(ContextState);
	// TODO: Shader Resources
	SetupTexturesForDraw(ContextState);
	CommitNonComputeShaderConstants(ContextState);
	CachedBindElementArrayBuffer(ContextState, IndexBuffer->GetGLResource());
	SetupVertexArrays(ContextState, BaseVertexIndex, mPendingState.Streams, NUM_OPENGL_VERTEX_STREAMS, NumVertices + StartIndex);

	GLenum IndexType = IndexBuffer->GetIndexSize() == sizeof(uint32_t) ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
	StartIndex *= IndexBuffer->GetIndexSize();

	if (NumInstances > 1) {
		glDrawElementsInstanced(GL_TRIANGLES, NumPrimitives * 3, IndexType, (void*)(uintptr_t)StartIndex, NumInstances);
	} else {
		glDrawRangeElements(GL_TRIANGLES, 0, NumVertices, NumPrimitives * 3, IndexType, (void*)(uintptr_t)StartIndex);
	}
	RHI_THREAD_END_COMMAND();
}