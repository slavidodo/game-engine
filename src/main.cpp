
#include <framework/pch.h>

#include <framework/platform/application.h>
#include <framework/platform/window.h>

const std::string WINDOW_TITLE = "Game Engine";
constexpr int32_t WINDOW_WIDTH = 1024;
constexpr int32_t WINDOW_HEIGHT = 768;

#if defined(DIRECTX12_RENDERER_BACKEND)
#	include <framework/graphics/directx12/context.h>
#	define RENDERING_BACKEND directx12
#elif defined(DIRECTX11_RENDERER_BACKEND)
#	include <framework/graphics/directx11/context.h>
#	define RENDERING_BACKEND directx11
#elif defined(VULKAN_RENDERER_BACKEND)
#	include <framework/graphics/vulkan/context.h>
#	define RENDERING_BACKEND vulkan
#else
#	include <framework/graphics/opengl/context.h>
#	define RENDERING_BACKEND opengl
#endif

using Context = framework::graphics::RENDERING_BACKEND::Context;

int main(int argc, char* argv[])
{
	std::vector<std::string> args(argv, argv + argc);

	uint32_t windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;

	if (!g_app.init())
		return 1;
	
	if (!g_window.init(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags))
		return 1;

	g_window.context(new Context());

	g_window.show();
	g_app.run();

	g_window.terminate();
	g_app.terminate();

	return 0;
}