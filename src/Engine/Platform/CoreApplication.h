
#ifndef ENGINE_PLATFORM_COREAPPLICATION_H
#define ENGINE_PLATFORM_COREAPPLICATION_H

class CoreApplication
{
public:
	CoreApplication() = default;
	CoreApplication(const CoreApplication&) = delete;
	CoreApplication& operator=(const CoreApplication&) = delete;

	bool Init();
	void Terminate();

	void RunMainLoop();

protected:
	static void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void OnMouseCallback(GLFWwindow* window, double xPosition, double yPosition);
	static void OnFramebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	bool mInitialized = false;
};

extern CoreApplication gApplication;

#endif // ENGINE_PLATFORM_COREAPPLICATION_H