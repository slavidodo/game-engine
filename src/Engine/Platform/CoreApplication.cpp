
#include "pch.h"

#include "CoreApplication.h"
#include "Window.h"

#include "../Filesystem/ResourceManager.h"
#include "../Scene/SceneManager.h"

CoreApplication g_app;

bool CoreApplication::Init()
{
	if (m_initialized)
		return false;

	// TODO; error codes based on faliure
	if (!ResourceManager::GetInstance().Init()) {
		std::cout << "Resource manager failed to load!" << std::endl;
		return false;
	}

	glfwInit();

	m_initialized = true;
	return true;
}

void CoreApplication::Terminate()
{
	if (!m_initialized)
		return;

	ResourceManager::GetInstance().Terminate();

	m_initialized = false;
}

void CoreApplication::RunMainLoop()
{
	GLFWwindow* window = g_window.glfwWindow();
	RHIContext* context = g_window.context();

	glfwSetKeyCallback(window, &CoreApplication::onKeyCallback);
	glfwSetFramebufferSizeCallback(window, &CoreApplication::onFramebufferSizeCallback);

	while (!glfwWindowShouldClose(g_window.glfwWindow())) {
		// begin frame data
		context->BeginRenderFrame();

		// render current scene
		SceneManager::GetInstance().Render();

		// end frame
		context->EndRenderFrame();

		// poll events
		glfwPollEvents();
	}
}

void CoreApplication::onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//g_inputManager.onKeyCallback(key, scancode, action, mods);
}

void CoreApplication::onFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	g_window.setWindowSize(glm::uvec2(width, height));
}