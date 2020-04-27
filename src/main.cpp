
#include "pch.h"

#include "Engine/Platform/CoreApplication.h"
#include "Engine/Platform/Window.h"
#include "Engine/Filesystem/ResourceManager.h"
#include <physfs.h>

const std::string WINDOW_TITLE = "Game Engine";
constexpr int32_t WINDOW_WIDTH = 1024;
constexpr int32_t WINDOW_HEIGHT = 768;

#include "Engine/OpenGL3RHI/OpenGL3RHIContext.h"

#if defined(DIRECTX12_RENDERER_BACKEND)
#	include "Engine/D3D12RHI/D3D12RHIContext.h"
#	define RENDERING_BACKEND directx12::D3D12Context
#elif defined(DIRECTX11_RENDERER_BACKEND)
#	include "Engine/D3D12RHI/D3D11RHIContext.h"
#	define RENDERING_BACKEND directx11::D3D11Context
#elif defined(VULKAN_RENDERER_BACKEND)
#	include "Engine/VulkanRHI/VulkanRHIContext.h"
#	define RENDERING_BACKEND VulkanRHIContext
#else
#	define RENDERING_BACKEND OpenGL3RHIContext
#	define RENDERING_OPENGL3_FALLBACK
#endif

using Context = RENDERING_BACKEND;

int main(int argc, char* argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	// initialize core app
	if (!g_app.Init())
		return 1;

	// initialize window
	if (!g_window.Init(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT))
		return 1;

	// TODO the whole process of creating a window should involve creating its own context
	// resources and manage it on its Init/Terminate

	GLFWwindow* glfwWindow = g_window.GetGlfwWindow();
	RHIContext* context = new Context(glfwWindow);

	if (!context->Init()) {
		delete context;

#ifndef RENDERING_OPENGL3_FALLBACK
		context = new OpenGL3RHIContext(glfwWindow);
		if (!context->Init()) {
			delete context;
			return 1;
		}
#else
		return 1;
#endif
	}

	// set the main context of the window
	g_window.SetContext(context);

	// show window
	g_window.show();

	// run application's main loop
	g_app.RunMainLoop();

	// shut down, remove context and related resources
	g_window.SetContext(nullptr);
	context->Terminate();
	delete context;

	// terminate window and core app
	g_window.Terminate();
	g_app.Terminate();

	return 0;
}