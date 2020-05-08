
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

	void Render(RHICommandList& RHICmdList);

	void SetCurrentScene(Scene_ptr scene) {
		mCurrentScene = scene;
	}
private:
	Scene_ptr mCurrentScene;
};

#endif // ENGINE_SCENE_SCENEMANAGER_H