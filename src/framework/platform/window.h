
#ifndef FRAMEWORK_PLATFORM_WINDOW_H
#define FRAMEWORK_PLATFORM_WINDOW_H

#include <framework/graphics/core/context.h>

namespace framework {
namespace platform {

class Window
{
public:
	Window() = default;
	~Window();

	bool init(std::string title, int32_t width, int32_t height);
	void terminate();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void show();
	void hide();

	void setWindowSize(glm::uvec2 size);
	glm::uvec2 getWindowSize() { return m_size; }
	void updateWindowSize();
	void resize(glm::uvec2 size);

	void title(std::string value);
	const std::string title() const {
		return m_title;
	}

	OBJECT_GETACCESSOR(GLFWwindow*, const GLFWwindow* , glfwWindow);
	OBJECT_ACCESSOR(framework::graphics::core::Context*, const framework::graphics::core::Context* , context);

private:
	GLFWwindow* m_glfwWindow;
	framework::graphics::core::Context* m_context;
	std::string m_title;

	glm::uvec2 m_size;

	bool m_initialized = false;
};

}
}

extern framework::platform::Window g_window;

#endif