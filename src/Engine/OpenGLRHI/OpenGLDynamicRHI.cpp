
#include "pch.h"

#include "OpenGLDynamicRHI.h"

#include "../Platform/Window.h"

struct PlatformOpenGLDevice* PlatformDevice;

OpenGLDynamicRHI* PrivateOpenGLDevice = nullptr;

bool OpenGLDynamicRHI::Init()
{
	PrivateOpenGLDevice = this;

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
	//extern PlatformOpenGLDevice* PlatformCreateOpenGLDevice();
	//PlatformDevice = PlatformCreateOpenGLDevice();

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

	OpenGLSpecs::Init();
	InitializeStateResources();

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
	glDeleteVertexArrays(1, &mVertexArrayObject);

	glfwMakeContextCurrent(nullptr);

	PrivateOpenGLDevice = nullptr;
}

void OpenGLDynamicRHI::InitializeStateResources()
{
	mSharedContextState.InitializeResources();
	mRenderingContextState.InitializeResources();
	mPendingState.InitializeResources();
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
	OpenGLDynamicRHICleanVertexBuffer();
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

void OpenGLDynamicRHI::BindUniformBufferBase(OpenGLContextState& ContextState, int32_t NumUniformBuffers, RHIUniformBufferRef* BoundUniformBuffers, uint32_t FirstUniformBuffer, bool ForceUpdate)
{
	for (int32_t BufferIndex = 0; BufferIndex < NumUniformBuffers; ++BufferIndex) {
		GLuint Buffer = 0;
		uint32_t Offset = 0;
		uint32_t Size = ZERO_FILLED_DUMMY_UNIFORM_BUFFER_SIZE;
		int32_t BindIndex = FirstUniformBuffer + BufferIndex;

		if (BoundUniformBuffers[BufferIndex]) {
			OpenGLUniformBuffer* UniformBuffer = (OpenGLUniformBuffer*)BoundUniformBuffers[BufferIndex].GetReference();
			Buffer = UniformBuffer->Resource;
			Size = UniformBuffer->GetSize();
		} else {
			if (mPendingState.ZeroFilledDummyUniformBuffer == 0) {
				void* ZeroBuffer = malloc(ZERO_FILLED_DUMMY_UNIFORM_BUFFER_SIZE);
				memset(ZeroBuffer, 0, ZERO_FILLED_DUMMY_UNIFORM_BUFFER_SIZE);
				glGenBuffers(1, &mPendingState.ZeroFilledDummyUniformBuffer);
				CachedBindUniformBuffer(ContextState, mPendingState.ZeroFilledDummyUniformBuffer);
				glBufferData(GL_UNIFORM_BUFFER, ZERO_FILLED_DUMMY_UNIFORM_BUFFER_SIZE, ZeroBuffer, GL_STATIC_DRAW);
				free(ZeroBuffer);
			}

			Buffer = mPendingState.ZeroFilledDummyUniformBuffer;
		}

		if (ForceUpdate || (Buffer != 0 && ContextState.UniformBuffers[BindIndex] != Buffer) || ContextState.UniformBufferOffsets[BindIndex] != Offset) {
			glBindBufferRange(GL_UNIFORM_BUFFER, BindIndex, Buffer, Offset, Size);
			ContextState.UniformBuffers[BindIndex] = Buffer;
			ContextState.UniformBufferOffsets[BindIndex] = Offset;
			ContextState.UniformBufferBound = Buffer; // glBindBufferRange binds the buffer
		}
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
	//extern OpenGLContextType PlatformOpenGLCurrentContext(struct PlatformOpenGLDevice* Device);
	//OpenGLContextType ContextType = PlatformOpenGLCurrentContext(PlatformDevice);
	//if (ContextType == OpenGLContextType::Rendering) {
	//	return mRenderingContextState;
	//} else if (ContextType == OpenGLContextType::Shared) {
	//	return mSharedContextState;
	//} else if (ContextType == OpenGLContextType::Invalid) {
	//	return mInvalidContextState;
	//}
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

void OpenGLDynamicRHI::CachedBindUniformBuffer(OpenGLContextState& ContextState, GLuint Buffer)
{
	if (ContextState.UniformBufferBound != Buffer) {
		glBindBuffer(GL_UNIFORM_BUFFER, Buffer);
		ContextState.UniformBufferBound = Buffer;
	}
}