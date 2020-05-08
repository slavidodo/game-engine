
#include "pch.h"

#include "OpenGLDynamicRHI.h"

#include "../Platform/Window.h"

#include <bitset>

bool OpenGLDynamicRHI::Init()
{
	// TODO: use independant loader if we gonna support android/ios/...
	// I'm not sure if glfw supports opengl es
	// also we force "glew" so we need to handle an wrapper to either load
	// glew or gles

	// set opengl 3.3 as our minimum requirements
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// TODO: RHI should not contain any "context-related" information
	// if we are likely to support multiple contexts and this is so
	// common, we need to have a separate context class
	// indeed we are storing this in a generalized window class, but the purpose
	// is to actually have a "more" robust structure, and this window acts as the
	// the main context

	// a platform context for windows/linux/mac-os should contain
	// GLFWwindow* however, I'm aware of android support, so we could
	// separate the window class to be "EditorWindow" instead

	// EXAMPLE
	struct PlatformContext
	{
		GLint VertexArrayObject;
		bool Vsync; // sync interval
		GLFWwindow* Context;

		void MakeCurrent() {
			glfwMakeContextCurrent(Context);
		}
	};

	{
		// a window has been created already
		// just make it current (i.e wglMakeContextCurrent)
		glfwMakeContextCurrent(gWindow.GetGlfwWindow());

		// initialize glew for opengl functions
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			glfwMakeContextCurrent(nullptr);
			return false;
		}

		glGenVertexArrays(1, &mVertexArrayObject);
		glBindVertexArray(mVertexArrayObject);
	}

	SetVsync(false);

	// print out information about the GPU
#if defined(DEBUG) || !defined(NDEBUG) || defined(_DEBUG)
	std::cout
		<< "========== OpenGL Dynamic RHI ===========\n"
		<< "     Version: " << glGetString(GL_VERSION) << "\n"
		<< "      Vendor: " << glGetString(GL_VENDOR) << "\n"
		<< "    Renderer: " << glGetString(GL_RENDERER) << "\n"
		<< "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION ) << "\n"
		<< "=========================================" << std::endl;
#endif
	return true;
}

void OpenGLDynamicRHI::Terminate()
{

}

void OpenGLDynamicRHI::RHIBeginScene()
{
}

void OpenGLDynamicRHI::RHIEndScene()
{
}

void OpenGLDynamicRHI::RHIBeginFrame()
{
}

void OpenGLDynamicRHI::RHIEndFrame()
{
}

void OpenGLDynamicRHI::ClearScreen(bool color, bool depth)
{
	GLbitfield bits = 0;
	if (color)
		bits |= GL_COLOR_BUFFER_BIT;

	if (depth)
		bits |= GL_DEPTH_BUFFER_BIT;

	glClear(bits);
}

void OpenGLDynamicRHI::ClearColor(glm::fvec4 color)
{
	glClearColor(color.x, color.y, color.z, color.w);
	//glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
}

void OpenGLDynamicRHI::RHIDrawPrimitive(uint32_t BaseVertexIndex, uint32_t NumPrimitives, uint32_t NumInstances)
{
	OpenGLContextState& ContextState = GetContextStateForCurrentContext();
	BindPendingFramebuffer(ContextState);
	//SetPendingBlendStateForActiveRenderTargets(ContextState);
	UpdateViewportInOpenGLContext(ContextState);
	UpdateScissorRectInOpenGLContext(ContextState);
	UpdateRasterizerStateInOpenGLContext(ContextState);
	//UpdateDepthStencilStateInOpenGLContext(ContextState);
	BindPendingShaderState(ContextState);
	//CommitGraphicsResourceTables();
	//SetupTexturesForDraw(ContextState);
	//CommitNonComputeShaderConstants();
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
}

void OpenGLDynamicRHI::RHIDrawIndexedPrimitive(RHIIndexBuffer_ptr IndexBufferRHI, uint32_t BaseVertexIndex, uint32_t FirstInstance, uint32_t NumVertices, uint32_t StartIndex, uint32_t NumPrimitives, uint32_t NumInstances)
{
	auto IndexBuffer = std::dynamic_pointer_cast<OpenGLIndexBuffer>(IndexBufferRHI);

	OpenGLContextState& ContextState = GetContextStateForCurrentContext();

	BindPendingFramebuffer(ContextState);
	// TODO: Blending
	UpdateViewportInOpenGLContext(ContextState);
	UpdateScissorRectInOpenGLContext(ContextState);
	UpdateRasterizerStateInOpenGLContext(ContextState);
	// TODO: Depth Stencil
	BindPendingShaderState(ContextState);
	// TODO: Shader Resources
	// TODO: Textures
	// TODO: Uniform Buffers (IMPORTANT)
	CachedBindElementArrayBuffer(ContextState, IndexBuffer->GetBufferId());
	SetupVertexArrays(ContextState, BaseVertexIndex, mPendingState.Streams, NUM_OPENGL_VERTEX_STREAMS, NumVertices + StartIndex);

	GLenum IndexType = IndexBuffer->GetIndexSize() == sizeof(uint32_t) ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
	StartIndex *= IndexBuffer->GetIndexSize();

	if (NumInstances > 1) {
		glDrawElementsInstanced(GL_TRIANGLES, NumPrimitives * 3, IndexType, (void*)StartIndex, NumInstances);
	} else {
		glDrawRangeElements(GL_TRIANGLES, 0, NumVertices, NumPrimitives * 3, IndexType, (void*)StartIndex);
	}
}

void OpenGLDynamicRHI::RHISetStreamSource(uint32_t StreamIndex, RHIVertexBuffer_ptr VertexBufferRHI, uint32_t Offset)
{
	auto VertexBuffer = std::dynamic_pointer_cast<OpenGLVertexBuffer>(VertexBufferRHI);

	mPendingState.Streams[StreamIndex].VertexBuffer = VertexBuffer;
	mPendingState.Streams[StreamIndex].Stride = 0;
	mPendingState.Streams[StreamIndex].Offset = Offset;
}

void OpenGLDynamicRHI::RHISetViewport(glm::uvec2 Minimum, glm::uvec2 Maximum)
{
	mPendingState.Viewport = IntRect(Minimum, Maximum);

	RHISetScissorRect(false, glm::uvec2(0), glm::uvec2(0));
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

void OpenGLDynamicRHI::RHISetBoundShaderState(RHIBoundShaderState_ptr BoundShaderStateRHI)
{
	auto BoundShaderState = std::dynamic_pointer_cast<OpenGLBoundShaderState>(BoundShaderStateRHI);
	mPendingState.BoundShaderState = BoundShaderState;
}

// TODO move these to their respective code files

void OpenGLDynamicRHI::UpdateViewportInOpenGLContext(OpenGLContextState& ContextState)
{
	if (ContextState.Viewport != mPendingState.Viewport) {
		IntRect::TPoint position = mPendingState.Viewport.GetMinimum();
		IntRect::TPoint size = mPendingState.Viewport.GetSize();

		glViewport(position.x, position.y, size.x, size.y);
		ContextState.Viewport = mPendingState.Viewport;
	}
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

void OpenGLDynamicRHI::BindPendingShaderState(OpenGLContextState& ContextState)
{
	OpenGLLinkedProgram_ptr LinkedProgram = mPendingState.BoundShaderState->GetLinkedProgram();
	GLuint PendingProgram = LinkedProgram->GetProgram();

	if (ContextState.Program != PendingProgram) {
		glUseProgram(PendingProgram);
		ContextState.Program = PendingProgram;
	}
}

GLenum OpenGLDynamicRHI::GetGLUsageForHardwareUsage(RHIHardwareBufferUsage hbUsage)
{
	if (hbUsage & RHIHardwareBufferUsage::HWBU_DISCARD) {
		return GL_STREAM_DRAW;
	} else if (hbUsage & RHIHardwareBufferUsage::HWBU_STATIC) {
		return GL_STATIC_DRAW;
	} else {
		return GL_DYNAMIC_DRAW;
	}
}

OpenGLContextState& OpenGLDynamicRHI::GetContextStateForCurrentContext()
{
	return mRenderingContextState;
}

void OpenGLDynamicRHI::SetupVertexArrays(OpenGLContextState& ContextState, uint32_t BaseVertexIndex, OpenGLStream* Streams, uint32_t NumStreams, uint32_t MaxVertices)
{
	// TODO: if we support VAB, there would be a different path
	if (GLEW_VERSION_4_3 || GLEW_ARB_vertex_attrib_binding) {
		// VAB
	}

	uint32_t UsedAttributes = 0;

	auto VertexDeclaration = mPendingState.BoundShaderState->GetVertexDeclaration();
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
									  Stream->VertexBuffer->GetVertexSize(),
									  (void*)((uint32_t)(BaseVertexIndex * Stream->Stride + Stream->Offset + VertexElement.Offset)),
									  Stream->VertexBuffer->GetBufferId());

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

void OpenGLDynamicRHI::CachedBindArrayBuffer(OpenGLContextState& ContextState, GLuint Buffer)
{
	if (ContextState.ArrayBufferBound != Buffer) {
		glBindBuffer(GL_ARRAY_BUFFER, Buffer);
		ContextState.ArrayBufferBound = Buffer;
	}
}

void OpenGLDynamicRHI::CachedBindElementArrayBuffer(OpenGLContextState& ContextState, GLuint Buffer)
{
	if (ContextState.ElementArrayBufferBound != Buffer) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer);
		ContextState.ElementArrayBufferBound = Buffer;
	}
}