
#include "pch.h"

#include "application.h"

#include <framework/platform/window.h>
#include <framework/managers/scene_manager.h>
#include <framework/managers/input_manager.h>
#include <framework/managers/resource_manager.h>

using namespace framework::platform;

Application g_app;

bool Application::init()
{
	if (m_initialized)
		return false;

	// TODO; error codes based on faliure
	if (!g_resourceManager.init()) {
		std::cout << "Resource manager failed to load!" << std::endl;
		return false;
	}

	glfwInit();

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
	GLFWwindow* window = g_window.glfwWindow();
	framework::graphics::core::Context* context = g_window.context();

	glfwSetKeyCallback(window, &Application::onKeyCallback);
	glfwSetFramebufferSizeCallback(window, &Application::onFramebufferSizeCallback);

	while (!glfwWindowShouldClose(g_window.glfwWindow())) {
		// begin frame data
		context->beginFrame();

		// render current scene
		g_sceneManager.render();

		// end frame
		context->endFrame();

		// poll events
		glfwPollEvents();
	}
}

void Application::onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//g_inputManager.onKeyCallback(key, scancode, action, mods);
}

void Application::onFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	g_window.setWindowSize(glm::uvec2(width, height));
}