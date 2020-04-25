
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
	glm::uvec2 getWindowSize() { return mSize; }
	void updateWindowSize();
	void resize(glm::uvec2 size);

	void title(std::string value);
	const std::string title() const {
		return mTitle;
	}

	OBJECT_GETACCESSOR(GLFWwindow*, const GLFWwindow* , GlfwWindow);
	OBJECT_ACCESSOR(RHIContext*, const RHIContext* , Context);

private:
	GLFWwindow* mGlfwWindow;
	RHIContext* mContext;
	std::string mTitle;

	glm::uvec2 mSize;

	bool mInitialized = false;
};

extern Window g_window;

#endif // ENGINE_PLATFORM_WINDOW_H