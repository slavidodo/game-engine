
#ifndef ENGINE_INPUT_INPUTMANAGER_H
#define ENGINE_INPUT_INPUTMANAGER_H

#include "../Scene/SceneManager.h"
#include "../Platform/Window.h"
#include "../Utils.h"

class InputManager
{
public:
	static InputManager& GetInstance() {
		static InputManager instance;
		return instance;
	}

	void OnKeyCallback(int32_t key, int32_t scancode, int32_t action, int32_t mods);
	void OnMouseCallback(double xPosition, double yPosition);

	void CheckInputs();

private:
	std::map<int32_t, uint32_t> mKeys;

	bool bFirstFrame = true;
	std::pair<double, double> mMousePos;
};

#endif // ENGINE_INPUT_INPUTMANAGER_H