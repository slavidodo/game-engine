
#ifndef ENGINE_INPUT_INPUTMANAGER_H
#define ENGINE_INPUT_INPUTMANAGER_H

class InputManager
{
public:
	void onKeyCallback(int32_t key, int32_t scancode, int32_t action, int32_t mods);

private:
	std::map<int32_t, uint32_t> mKeys;
};

#endif // ENGINE_INPUT_INPUTMANAGER_H