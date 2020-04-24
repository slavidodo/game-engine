
#include "pch.h"

#include "OpenGL3RHIContext.h"
#include "OpenGL3RHIBuffer.h"

OpenGL3RHIContext::OpenGL3RHIContext(GLFWwindow* window)
	: RHIContext(window)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

void OpenGL3RHIContext::DrawMeshWithoutOptimization(Mesh_ptr mesh, Material_ptr material, Camera_ptr camera)
{
	/*
	// obtain descriptor and upload gpu data if not yet
	auto descriptor = std::dynamic_pointer_cast<OpenGLMeshDescriptor>(mesh->descriptor());
	descriptor->update();

	// put the shader to use
	shader->use();
	shader->setMat4x4("GameEngine_MVP", mvp);

	// bind vao and issue a draw call
	glBindVertexArray(descriptor->VAO());
	glDrawElements(GL_TRIANGLES, (GLsizei)descriptor->mesh()->indices().size(), GL_UNSIGNED_INT, 0);

	// reset vao binding
	glBindVertexArray(0);*/
}

void OpenGL3RHIContext::BeginRenderFrame()
{

}

void OpenGL3RHIContext::EndRenderFrame()
{
	// swap buffers
	glfwSwapBuffers(mWindow);
}