
#ifndef ENGINE_SCENE_SCENE_RENDERER_H
#define ENGINE_SCENE_SCENE_RENDERER_H

#include "RScene.h"

class SceneRenderer
{
public:
	RScene_ptr GetScene() {
		return mScene;
	}
	
	void InitRHI(RHICommandList& RHICmdList);
	void Render(RHICommandList& RHICmdList);

private:
	RScene_ptr mScene;
};

#endif // ENGINE_SCENE_SCENE_RENDERER_H