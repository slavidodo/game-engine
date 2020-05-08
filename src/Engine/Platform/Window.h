
#ifndef ENGINE_PLATFORM_WINDOW_H
#define ENGINE_PLATFORM_WINDOW_H

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
	void resize(glm::uvec2 size);

	void title(std::string value);
	const std::string title() const {
		return mTitle;
	}

	OBJECT_GETACCESSOR(GLFWwindow*, const GLFWwindow* , GlfwWindow);

private:
	GLFWwindow* mGlfwWindow;
	std::string mTitle;

	glm::uvec2 mSize;

	bool mInitialized = false;
};

extern Window gWindow;

#endif // ENGINE_PLATFORM_WINDOW_H