
#include "pch.h"

#include "CoreApplication.h"
#include "Window.h"

#include "../Filesystem/ResourceManager.h"
#include "../Physics/PhysicsEngine.h"
#include "../Input/InputManager.h"
#include "../Scene/SceneManager.h"
#include "../RHI/RHICommandList.h"
#include "../Globals.h"

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

	PhysicsSettings settings;
	{
		#ifdef _DEBUG
		settings.bEnableVisualDebugger = true;
		settings.bTrackMemoryAllocations = true;
		#endif
		//settings.toleranceScale.length = 2.4f;
		settings.bEnableHeightFields = true;
	}
	if (!PhysicsEngine::GetInstance().Init(settings)) {
		std::cout << "Physics engine failed to initialize!" << std::endl;
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

	PhysicsEngine::GetInstance().Terminate();
	ResourceManager::GetInstance().Terminate();

	mInitialized = false;
}

void CoreApplication::RunMainLoop()
{
	GLFWwindow* glfwWindow = gWindow.GetGlfwWindow();

	glfwSetKeyCallback(glfwWindow, &CoreApplication::OnKeyCallback);
	glfwSetCursorPosCallback(glfwWindow, &CoreApplication::OnMouseCallback);
	glfwSetFramebufferSizeCallback(glfwWindow, &CoreApplication::OnFramebufferSizeCallback);

	// todo: this is just temporary, since we don't really support async commands
	// moreover, we don't really track the sequence of commands
	RHICommandListImmediate& RHICmdList = RHICommandListExecutor::GetImmediateCommandList();
	while (!glfwWindowShouldClose(gWindow.GetGlfwWindow())) {
		RHICmdList.BeginFrame();
		{
			InputManager::GetInstance().CheckInputs();
			SceneManager::GetInstance().UpdatePhysics();
			SceneManager::GetInstance().Render(RHICmdList);
		}
		RHICmdList.EndFrame();

		// poll events
		glfwPollEvents();

		gFrameNumber++;

		/*int frameX, frameY;
		glfwGetFramebufferSize(gWindow.GetGlfwWindow(), &frameX, &frameY);
		std::cout << "Framebuffer Size: (" << frameX << ", " << frameY << ")\n";

		int windowX, windowY;
		glfwGetWindowSize(gWindow.GetGlfwWindow(), &windowX, &windowY);
		std::cout << "Window Size: (" << windowX << ", " << windowY << ")\n";

		float xscale, yscale;
		glfwGetWindowContentScale(gWindow.GetGlfwWindow(), &xscale, &yscale);
		std::cout << "Scale: (" << xscale << ", " << yscale << ")\n";*/

	}
}

void CoreApplication::OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputManager::GetInstance().OnKeyCallback(key, scancode, action, mods);
}

void CoreApplication::OnMouseCallback(GLFWwindow* window, double xPosition, double yPosition) {
	InputManager::GetInstance().OnMouseCallback(xPosition, yPosition);
}

void CoreApplication::OnFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	gWindow.SetWindowSize(glm::uvec2(width, height));
}