
#ifndef FRAMEWORK_GRAPHICS_OPENGL_CONTEXT
#define FRAMEWORK_GRAPHICS_OPENGL_CONTEXT

#include <framework/graphics/core/context.h>

#include <SDL2/SDL_video.h>

namespace framework {
namespace graphics {
namespace opengl {

class Context : public core::Context
{
public:
	Context();
	~Context();

	SDL_GLContext sdlContext() { return m_sdlContext; }
	const SDL_GLContext sdlContext() const { return m_sdlContext; }

	void vsync(bool value) override final {
		SDL_GL_SetSwapInterval(value);
	}
	bool vsync() override final {
		return SDL_GL_GetSwapInterval();
	}
	bool vsync() const override final {
		return SDL_GL_GetSwapInterval();
	}

	void clearColor(framework::core::Color color) override final;

private:
	SDL_GLContext m_sdlContext;
};

} // ns framework
} // ns graphics
} // ns opengl

#endif
