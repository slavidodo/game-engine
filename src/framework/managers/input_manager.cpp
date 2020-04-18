
#include <framework/pch.h>

#include "input_manager.h"

#include <iostream>

using namespace framework::managers;

InputManager g_inputManager;

void InputManager::onKeyDown(const SDL_KeyboardEvent& e)
{
	// value refers to last pressed time
	uint32_t& value = m_keys[e.keysym.sym];

	if (value != 0) {
		// repeat
	} else {
		// no repeat
	}

	value = SDL_GetTicks();
}

void InputManager::onKeyUp(const SDL_KeyboardEvent& e)
{
	uint32_t& value = m_keys[e.keysym.sym];
	value = 0;
}