
#ifndef FRAMEWORK_MANAGERS_INPUTMANAGER_H
#define FRAMEWORK_MANAGERS_INPUTMANAGER_H

namespace framework {
namespace managers {

class InputManager
{
public:
	void onKeyCallback(int32_t key, int32_t scancode, int32_t action, int32_t mods);

private:
	std::map<int32_t, uint32_t> m_keys;

};

} // ns managers
} // ns framework

extern framework::managers::InputManager g_inputManager;

#endif