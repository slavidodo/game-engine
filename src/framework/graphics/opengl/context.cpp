
#include <framework/pch.h>

#include "context.h"

#include <framework/graphics/core/context.h>
#include <framework/platform/window.h>
#include <GL/glew.h>

using namespace framework::core;
using namespace framework::graphics::opengl;

Context::Context()
	: framework::graphics::core::Context()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	m_sdlContext = SDL_GL_CreateContext(g_window.sdlWindow());
	if (m_sdlContext == nullptr)
		throw "failed to create a context";

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();

	if (glewError != GLEW_OK) {
		std::cout << "Error initializing GLEW! %s" << glewGetErrorString(glewError) << std::endl;
	}
}

Context::~Context()
{
	SDL_GL_DeleteContext(m_sdlContext);
}

void Context::clearColor(Color color)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(color.r(), color.g(), color.b(), color.a());
}