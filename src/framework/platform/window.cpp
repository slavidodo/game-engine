
#include "pch.h"

#include "window.h"

using namespace framework::platform;

Window g_window;

Window::~Window()
{
	terminate();
}

bool Window::init(std::string title, int32_t width, int32_t height)
{
	if (m_initialized)
		return false;

	m_size = glm::uvec2(width, height);
	m_title = title;

	// initialize glfw3 window
	m_glfwWindow = glfwCreateWindow(width, height, m_title.c_str(), nullptr, nullptr);
	if (m_glfwWindow == nullptr) {
		const char* err; glfwGetError(&err);
		std::cout << "Failed to create window, " << err << std::endl;
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

	glfwDestroyWindow(m_glfwWindow);
}

void Window::show()
{
	glfwShowWindow(m_glfwWindow);
}

void Window::hide()
{
	glfwHideWindow(m_glfwWindow);
}

void Window::setWindowSize(glm::uvec2 size)
{
	m_size = size;
	context()->resize(size);
}

void Window::updateWindowSize()
{
	glm::ivec2 size;
	glfwGetWindowSize(m_glfwWindow, &size.x, &size.y);
	setWindowSize(glm::uvec2(size));
}

void Window::resize(glm::uvec2 size)
{
	setWindowSize(size);
	glfwSetWindowSize(m_glfwWindow, size.x, size.y);
}

void Window::title(std::string value)
{
	m_title = value;
	glfwSetWindowTitle(m_glfwWindow, m_title.c_str());
}