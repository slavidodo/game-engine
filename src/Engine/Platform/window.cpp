
#include "pch.h"

#include "Window.h"

#include "../RHI/DynamicRHI.h"

Window gWindow;

Window::~Window()
{
	Terminate();
}

bool Window::Init(std::string title, int32_t width, int32_t height)
{
	if (mInitialized)
		return false;

	// TODO(honey): high-dpi fix. should work for now :)
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

	mSize = glm::uvec2(width, height);
	mTitle = title;

	// initialize glfw3 window
	mGlfwWindow = glfwCreateWindow(width, height, mTitle.c_str(), nullptr, nullptr);
	if (mGlfwWindow == nullptr) {
		const char* err; glfwGetError(&err);
		std::cout << "Failed to create window, " << err << std::endl;
		return false;
	}

	mInitialized = true;
	return true;
}

void Window::Terminate()
{
	if (!mInitialized)
		return;

	mInitialized = false;

	glfwDestroyWindow(mGlfwWindow);
}

void Window::show()
{
	glfwShowWindow(mGlfwWindow);
}

void Window::hide()
{
	glfwHideWindow(mGlfwWindow);
}

void Window::setWindowSize(glm::uvec2 size)
{
	mSize = size;
	gDynamicRHI->RHISetViewport(glm::uvec2(0), size);
}

void Window::resize(glm::uvec2 size)
{
	setWindowSize(size);
	glfwSetWindowSize(mGlfwWindow, size.x, size.y);
}

void Window::title(std::string value)
{
	mTitle = value;
	glfwSetWindowTitle(mGlfwWindow, mTitle.c_str());
}