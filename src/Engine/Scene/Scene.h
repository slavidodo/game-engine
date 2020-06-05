
#ifndef ENGINE_SCENE_SCENE_H
#define ENGINE_SCENE_SCENE_H

#include "SceneNode.h"
#include "../Core/Camera.h"
#include "../RHI/RHICommandList.h"

class Scene;
typedef std::shared_ptr<Scene> Scene_ptr;

class Scene
{
public:
	virtual void Render(RHICommandList& RHICmdList);

	Camera_ptr GetMainCamera() const {
		return mMainaCmera;
	}

protected:
	Camera_ptr mMainaCmera;
	SceneNode_ptr mRootNode;
};

#endif // ENGINE_SCENE_SCENE_H