
#include "pch.h"

#include "OpenGL3RHIContext.h"
#include "OpenGL3RHIResource.h"

OpenGL3RHIContext::OpenGL3RHIContext(GLFWwindow* window)
	: RHIContext(window)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		std::string str = "Error initializing GLEW: ";
		str += (char*)glewGetErrorString(glewError);
		throw str.c_str();
	}

	SetVsync(false);

	glEnable(GL_DEPTH_TEST); // todo; should be defined by shaders
	glEnable(GL_MULTISAMPLE); // todo; should be defined in project settings
}

bool OpenGL3RHIContext::Init()
{
	return true;
}

void OpenGL3RHIContext::Terminate()
{
}

void OpenGL3RHIContext::SetViewport(glm::uvec2 position, glm::uvec2 size)
{
	glViewport(position.x, position.y, size.x, size.y);
}

void OpenGL3RHIContext::ClearScreen(bool color, bool depth)
{
	GLbitfield bits = 0;
	if (color)
		bits |= GL_COLOR_BUFFER_BIT;

	if (depth)
		bits |= GL_DEPTH_BUFFER_BIT;

	glClear(bits);
}

void OpenGL3RHIContext::ClearColor(glm::fvec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGL3RHIContext::BeginRenderFrame()
{

}

void OpenGL3RHIContext::EndRenderFrame()
{
	// swap buffers
	glfwSwapBuffers(mWindow);
}

GLenum OpenGL3RHIContext::GetGLUsageForHardwareUsage(RHIHardwareBufferUsage hbUsage)
{
	if (hbUsage & RHIHardwareBufferUsage::HWBU_DISCARD) {
		return GL_STREAM_DRAW;
	} else if (hbUsage & RHIHardwareBufferUsage::HWBU_STATIC) {
		return GL_STATIC_DRAW;
	} else {
		return GL_DYNAMIC_DRAW;
	}
}