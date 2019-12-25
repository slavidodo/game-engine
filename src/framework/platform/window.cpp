
#include <framework/pch.h>

#include "window.h"

using namespace framework::platform;

Window g_window;

Window::~Window()
{
	if (m_window) {
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
}

bool Window::init(std::string title, int32_t width, int32_t height, uint32_t flags)
{
	if (m_initialized)
		return false;

	// initialize sdl window
	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if (m_window == nullptr)
		return false;

	m_initialized = true;
	return true;
}

void Window::terminate()
{
	if (!m_initialized)
		return;

	m_initialized = false;
}

void Window::show()
{
	SDL_ShowWindow(m_window);
}

void Window::hide()
{
	SDL_HideWindow(m_window);
}

void Window::swap()
{
	SDL_GL_SwapWindow(m_window);
}