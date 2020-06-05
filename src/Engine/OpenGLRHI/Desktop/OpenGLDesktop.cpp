
#include "pch.h"

#include "OpenGLDesktop.h"

#include "../OpenGLDefinitions.h"
#include <GLFW/glfw3.h>

struct PlatformOpenGLContext;
struct PlatformOpenGLDevice;

void PlatformCreateDummyWindow(PlatformOpenGLContext& Context);
void PlatformReleaseOpenGLContext(PlatformOpenGLDevice* Device, PlatformOpenGLContext* Context);

struct PlatformOpenGLContext
{
	GLFWwindow* Window;
	GLuint VertexArrayObject;
	GLuint ViewportFramebuffer;
};

class ContextScopedLock
{
public:
	ContextScopedLock(PlatformOpenGLContext Context) {
		mPreviousContext = glfwGetCurrentContext();
		mSameContext = (mPreviousContext == Context.Window);
		glfwMakeContextCurrent(Context.Window);
	}

	~ContextScopedLock() {
		if (!mSameContext) {
			glfwMakeContextCurrent(mPreviousContext);
		}
	}

private:
	GLFWwindow* mPreviousContext;
	bool mSameContext;
};

struct PlatformOpenGLDevice
{
	PlatformOpenGLContext RenderingContext;
	PlatformOpenGLContext SharedContext;

	PlatformOpenGLDevice() {
		// Shared Context
		PlatformCreateDummyWindow(SharedContext);
		{
			ContextScopedLock ScopeLock(SharedContext);
			glGenVertexArrays(1, &SharedContext.VertexArrayObject);
			glBindVertexArray(SharedContext.VertexArrayObject);
			glGenFramebuffers(1, &SharedContext.ViewportFramebuffer);
		}

		// Rendering Context
		PlatformCreateDummyWindow(RenderingContext);
		{
			ContextScopedLock ScopeLock(RenderingContext);
			glGenVertexArrays(1, &RenderingContext.VertexArrayObject);
			glBindVertexArray(RenderingContext.VertexArrayObject);
			glGenFramebuffers(1, &RenderingContext.ViewportFramebuffer);
		}
	}

	~PlatformOpenGLDevice() {
		glfwMakeContextCurrent(nullptr);

		PlatformReleaseOpenGLContext(this, &RenderingContext);
		PlatformReleaseOpenGLContext(this, &SharedContext);
	}
};

// Public functions
void PlatformCreateDummyWindow(PlatformOpenGLContext& Context)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	Context.Window = glfwCreateWindow(800, 600, "", NULL, NULL);
	if (Context.Window == NULL) {
		throw std::runtime_error("(context) window creation failed");
	}
}

void PlatformReleaseOpenGLContext(PlatformOpenGLDevice* Device, PlatformOpenGLContext* Context)
{
	glDeleteVertexArrays(1, &Context->VertexArrayObject);
	glDeleteFramebuffers(1, &Context->ViewportFramebuffer);
}

PlatformOpenGLDevice* PlatformCreateOpenGLDevice()
{
	return new PlatformOpenGLDevice;
}

OpenGLContextType PlatformOpenGLCurrentContext(PlatformOpenGLDevice* Device)
{
	GLFWwindow* Context = glfwGetCurrentContext();
	if (Context == Device->SharedContext.Window) {
		return OpenGLContextType::Shared;
	} else if (Context == Device->RenderingContext.Window) {
		return OpenGLContextType::Rendering;
	} else if (Context != NULL) {
		return OpenGLContextType::Other;
	} else {
		return OpenGLContextType::Invalid;
	}
}