
#ifndef FRAMEWORK_PLATFORM_APPLICATION_H
#define FRAMEWORK_PLATFORM_APPLICATION_H

#include <SDL2/SDL.h>

namespace framework {
namespace platform {

class Application
{
public:
	Application() = default;

	bool init();
	void terminate();

	void run();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

protected:

	void handleEvent(const SDL_Event& e);

private:
	bool m_initialized = false;
};

} // ns framework
} // ns platform

extern framework::platform::Application g_app;

#endif