
#include "pch.h"

#include "window.h"

using namespace framework::platform;

Window g_window;

Window::~Window()
{
	if (m_sdlWindow) {
		SDL_DestroyWindow(m_sdlWindow);
		m_sdlWindow = nullptr;
	}
}

bool Window::init(std::string title, int32_t width, int32_t height, uint32_t flags)
{
	if (m_initialized)
		return false;

	m_size = glm::uvec2(width, height);

	// initialize sdl window
	m_sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if (m_sdlWindow == nullptr) {
		std::cout << "Failed to create window, " << SDL_GetError() << std::endl;
		return false;
	}

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
	SDL_ShowWindow(m_sdlWindow);
}

void Window::hide()
{
	SDL_HideWindow(m_sdlWindow);
}

void Window::setWindowSize(glm::uvec2 size)
{
	m_size = size;
	context()->resize(size);
}

void Window::updateWindowSize()
{
	glm::ivec2 size;
	SDL_GetWindowSize(m_sdlWindow, &size.x, &size.y);
	setWindowSize(glm::uvec2(size));
}

void Window::resize(glm::uvec2 size)
{
	setWindowSize(size);
	SDL_SetWindowSize(m_sdlWindow, size.x, size.y);
}