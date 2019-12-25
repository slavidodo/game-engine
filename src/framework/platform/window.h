
#ifndef FRAMEWORK_PLATFORM_WINDOW_H
#define FRAMEWORK_PLATFORM_WINDOW_H

#include <framework/graphics/core/context.h>
#include <SDL2/SDL.h>

namespace framework {
namespace platform {

class Window
{
public:
	Window() = default;
	~Window();

	bool init(std::string title, int32_t width, int32_t height, uint32_t flags);
	void terminate();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void show();
	void hide();

	SDL_Window* sdlWindow() { return m_window; }
	const SDL_Window* sdlWindow() const { return m_window; }

	OBJECT_ACCESSOR(framework::graphics::core::Context*, context);

	void swap();

private:

	SDL_Window* m_window;
	framework::graphics::core::Context* m_context;

	bool m_initialized = false;
};

}
}

extern framework::platform::Window g_window;

#endif