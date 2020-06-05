
#ifndef ENGINE_SCENE_SCENE_RENDERER_H
#define ENGINE_SCENE_SCENE_RENDERER_H

#include "Scene.h"

class SceneRenderer
{
public:
	Scene_ptr GetScene() {
		return mScene;
	}
	
	void InitRHI(RHICommandList& RHICmdList);
	void Render(RHICommandList& RHICmdList);

private:
	Scene_ptr mScene;
};

#endif // ENGINE_SCENE_SCENE_RENDERER_H