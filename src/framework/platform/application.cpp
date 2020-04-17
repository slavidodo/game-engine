
#include <framework/pch.h>

#include "application.h"

#include <framework/platform/window.h>
#include <framework/managers/scene_manager.h>
#include <framework/managers/input_manager.h>

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

		// begin frame data
		auto context = g_window.context();
		context->beginFrame();

		// render current scene
		g_sceneManager.render();

		// end frame
		context->endFrame();
	}
}

void Application::handleEvent(const SDL_Event& e)
{
	switch (e.type) {
		case SDL_QUIT:
			//onQuit();
			break;
		case SDL_APP_TERMINATING:
			break;
		case SDL_APP_LOWMEMORY:
			break;

		case SDL_WINDOWEVENT:
			handleWindowEvent(e.window);
			break;

		case SDL_KEYDOWN:
			g_inputManager.onKeyDown(e.key);
			break;
		case SDL_KEYUP:
			g_inputManager.onKeyUp(e.key);
			break;
		case SDL_KEYMAPCHANGED:
			//g_inputManager.onKeymapChanged(e);
			break;
		case SDL_MOUSEMOTION:
			//g_inputManager.onMouseMotion(e);
			break;
		case SDL_MOUSEBUTTONDOWN:
			break;
		case SDL_MOUSEBUTTONUP:
			break;
		case SDL_MOUSEWHEEL:
			break;

	}
}

void Application::handleWindowEvent(const SDL_WindowEvent& e)
{
	switch (e.event) {
		case SDL_WINDOWEVENT_RESIZED: {
			g_window.setWindowSize(glm::uvec2(e.data1, e.data2));
			break;
		}

		case SDL_WINDOWEVENT_MAXIMIZED:
		case SDL_WINDOWEVENT_RESTORED: {
			g_window.updateWindowSize();
			break;
		}
	}
}