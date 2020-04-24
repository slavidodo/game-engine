
#ifndef ENGINE_PLATFORM_WINDOW_H
#define ENGINE_PLATFORM_WINDOW_H

#include "../RHI/RHIContext.h"

class Window
{
public:
	Window() = default;
	~Window();

	bool Init(std::string title, int32_t width, int32_t height);
	void Terminate();

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
	OBJECT_ACCESSOR(RHIContext*, const RHIContext* , context);

private:
	GLFWwindow* m_glfwWindow;
	RHIContext* m_context;
	std::string m_title;

	glm::uvec2 m_size;

	bool m_initialized = false;
};

extern Window g_window;

#endif // ENGINE_PLATFORM_WINDOW_H