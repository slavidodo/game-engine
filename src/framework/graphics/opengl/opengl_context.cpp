
#include <framework/pch.h>

#include "opengl_context.h"

#include "opengl_shader.h"
#include "opengl_hardware_buffer_manager.h"
#include <framework/platform/window.h>
#include <framework/managers/hardware_buffer_manager.h>
#include <GL/glew.h>

using namespace framework::core;
using namespace framework::graphics::opengl;

OpenGLContext::OpenGLContext()
	: framework::graphics::core::Context()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	m_sdlContext = SDL_GL_CreateContext(g_window.sdlWindow());
	if (m_sdlContext == nullptr)
		throw "failed to create a context!";

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();

	if (glewError != GLEW_OK) {
		std::string str = "Error initializing GLEW: ";
		str += (char*)glewGetErrorString(glewError);
		throw str.c_str();
	}

	glEnable(GL_DEPTH_TEST); // todo; should be defined by shaders
	glEnable(GL_MULTISAMPLE); // todo; should be defined in project settings
}

OpenGLContext::~OpenGLContext()
{
	SDL_GL_DeleteContext(m_sdlContext);
}

void OpenGLContext::init()
{
	framework::managers::HardwareBufferManager::instance(new OpenGLHardwareBufferManager);
}

void OpenGLContext::clear(bool color, bool depth)
{
	GLbitfield bits = 0;
	if (color)
		bits |= GL_COLOR_BUFFER_BIT;

	if (depth)
		bits |= GL_DEPTH_BUFFER_BIT;

	glClear(bits);
}

void OpenGLContext::clearColor(glm::fvec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLContext::draw(framework::core::Mesh_ptr mesh, framework::core::Material_ptr material,
						 framework::graphics::Camera_ptr camera)
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

void OpenGLContext::resize(glm::uvec2 size)
{
	glViewport(0, 0, size.x, size.y);
}

void OpenGLContext::beginFrame()
{

}

void OpenGLContext::endFrame()
{
	// swap buffers
	SDL_GL_SwapWindow(g_window.sdlWindow());
}