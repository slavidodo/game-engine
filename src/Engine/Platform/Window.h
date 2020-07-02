
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

	void Show();
	void Hide();

	void SetWindowSize(glm::uvec2 size);
	glm::uvec2 GetWindowSize() { return mSize; }
	void Resize(glm::uvec2 size);

	void Title(std::string value);
	const std::string Title() const {
		return mTitle;
	}

	GLFWwindow* GetGlfwWindow() const { return mGlfwWindow; }

private:
	GLFWwindow* mGlfwWindow;
	std::string mTitle;

	glm::uvec2 mSize;

	bool mInitialized = false;
};

extern Window gWindow;

#endif // ENGINE_PLATFORM_WINDOW_H