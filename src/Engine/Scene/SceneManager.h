
#ifndef ENGINE_SCENE_SCENEMANAGER_H
#define ENGINE_SCENE_SCENEMANAGER_H

#include "Scene.h"

class SceneManager
{
public:
	static SceneManager& GetInstance() {
		static SceneManager instance;
		return instance;
	}

	void Render();

private:
	Scene_ptr m_currentScene;
};

#endif // ENGINE_SCENE_SCENEMANAGER_H