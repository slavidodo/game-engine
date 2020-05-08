
#include "pch.h"

#include "CoreApplication.h"
#include "Window.h"

#include "../Filesystem/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include "../RHI/RHICommandList.h"

CoreApplication gApplication;

bool CoreApplication::Init()
{
	if (mInitialized)
		return false;

	// TODO; error codes based on faliure
	if (!ResourceManager::GetInstance().Init()) {
		std::cout << "Resource manager failed to load!" << std::endl;
		return false;
	}

	glfwInit();

	mInitialized = true;
	return true;
}

void CoreApplication::Terminate()
{
	if (!mInitialized)
		return;

	ResourceManager::GetInstance().Terminate();

	mInitialized = false;
}

void CoreApplication::RunMainLoop()
{
	GLFWwindow* glfwWindow = gWindow.GetGlfwWindow();

	glfwSetKeyCallback(glfwWindow, &CoreApplication::onKeyCallback);
	glfwSetFramebufferSizeCallback(glfwWindow, &CoreApplication::onFramebufferSizeCallback);

	// todo: this is just temporary, since we don't really support async commands
	// moreover, we don't really track the sequence of commands
	auto& RHICmdList = RHICommandListExecutor::GetImmediateCommandList();
	while (!glfwWindowShouldClose(gWindow.GetGlfwWindow())) {
		RHICmdList.BeginFrame();
		{
			SceneManager::GetInstance().Render(RHICmdList);
		}
		RHICmdList.EndFrame();

		// poll events
		glfwPollEvents();
	}
}

void CoreApplication::onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//gInputManager.onKeyCallback(key, scancode, action, mods);
}

void CoreApplication::onFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	gWindow.setWindowSize(glm::uvec2(width, height));
}