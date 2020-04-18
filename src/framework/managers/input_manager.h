
#ifndef FRAMEWORK_MANAGERS_INPUTMANAGER_H
#define FRAMEWORK_MANAGERS_INPUTMANAGER_H

#include <SDL2/SDL.h>

namespace framework {
namespace managers {

class InputManager
{
public:
	void onKeyDown(const SDL_KeyboardEvent& e);
	void onKeyUp(const SDL_KeyboardEvent& e);

private:
	std::map<SDL_Keycode, uint32_t> m_keys;

};

} // ns managers
} // ns framework

extern framework::managers::InputManager g_inputManager;

#endif