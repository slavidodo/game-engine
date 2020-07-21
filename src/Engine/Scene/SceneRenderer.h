
#ifndef ENGINE_SCENE_SCENE_RENDERER_H
#define ENGINE_SCENE_SCENE_RENDERER_H

#include "RenderScene.h"

class SceneRenderer
{
public:
	RenderScene_ptr GetScene() {
		return mScene;
	}
	
	void InitRHI(RHICommandList& RHICmdList);
	void Render(RHICommandList& RHICmdList);

private:
	RenderScene_ptr mScene;
};

#endif // ENGINE_SCENE_SCENE_RENDERER_H