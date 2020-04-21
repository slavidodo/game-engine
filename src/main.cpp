
#include <framework/pch.h>

#include <framework/platform/application.h>
#include <framework/platform/window.h>

const std::string WINDOW_TITLE = "Game Engine";
constexpr int32_t WINDOW_WIDTH = 1024;
constexpr int32_t WINDOW_HEIGHT = 768;

#include <framework/graphics/opengl/opengl_context.h>

#if defined(DIRECTX12_RENDERER_BACKEND)
#	include <framework/graphics/d3d12/d3d12_context.h>
#	define RENDERING_BACKEND directx12::D3D12Context
#elif defined(DIRECTX11_RENDERER_BACKEND)
#	include <framework/graphics/d3d11/d3d11_context.h>
#	define RENDERING_BACKEND directx11::D3D11Context
#elif defined(VULKAN_RENDERER_BACKEND)
#	include <framework/graphics/vulkan/vulkan_context.h>
#	define RENDERING_BACKEND vulkan::VulkanContext
#else
#	define RENDERING_BACKEND opengl::OpenGLContext
#	define RENDERING_OPENGL_FALLBACK
#endif

using Context = framework::graphics::RENDERING_BACKEND;

int main(int argc, char* argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	uint32_t defaultWindowFlags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;
	uint32_t windowFlags = Context::WindowFlags() | defaultWindowFlags;

	if (!g_app.init())
		return 1;

	// this will mostly happen when compiling using vulkan
	// and it's not actually supported in SDL2
	if (g_window.init(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags))
		g_window.context(new Context());
#ifndef RENDERING_OPENGL_FALLBACK
	else if (g_window.init(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, framework::graphics::opengl::OpenGLContext::WindowFlags() | defaultWindowFlags))
		g_window.context(new framework::graphics::opengl::OpenGLContext());
#endif
	else
		return 1;

	g_window.context()->init();

	g_window.show();
	g_app.run();

	g_window.terminate();
	g_app.terminate();

	return 0;
}