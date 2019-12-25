
#include <framework/pch.h>

#include "application.h"

#include <framework/platform/window.h>

using namespace framework::platform;

Application g_app;

bool Application::init()
{
	if (m_initialized)
		return false;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL Error: %s" << SDL_GetError() << std::endl;
		return false;
	}

	m_initialized = true;
	return true;
}

void Application::terminate()
{
	if (!m_initialized)
		return;

	m_initialized = false;
}

void Application::run()
{
	SDL_Event event;
	bool quitting = false;
	while (!quitting) {
		// handle queued events
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT)
				quitting = true;

			handleEvent(event);
		}

		// render
		g_window.context()->clearColor(core::Color::red);
		g_window.swap();
	}
}

void Application::handleEvent(const SDL_Event& e)
{

}