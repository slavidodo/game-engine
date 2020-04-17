
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

	void setWindowSize(glm::uvec2 size);
	glm::uvec2 getWindowSize() { return m_size; }
	void updateWindowSize();
	void resize(glm::uvec2 size);

	const std::string title() const {
		return SDL_GetWindowTitle(m_sdlWindow);
	}

	OBJECT_GETACCESSOR(SDL_Window*, const SDL_Window* , sdlWindow);
	OBJECT_ACCESSOR(framework::graphics::core::Context*, const framework::graphics::core::Context* , context);

private:

	SDL_Window* m_sdlWindow;
	framework::graphics::core::Context* m_context;

	glm::uvec2 m_size;

	bool m_initialized = false;
};

}
}

extern framework::platform::Window g_window;

#endif