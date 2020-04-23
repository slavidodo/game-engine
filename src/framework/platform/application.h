
#ifndef FRAMEWORK_PLATFORM_APPLICATION_H
#define FRAMEWORK_PLATFORM_APPLICATION_H

namespace framework {
namespace platform {

class Application
{
public:
	Application() = default;
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	bool init();
	void terminate();

	void run();

protected:
	static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void onFramebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	bool m_initialized = false;
};

} // ns framework
} // ns platform

extern framework::platform::Application g_app;

#endif