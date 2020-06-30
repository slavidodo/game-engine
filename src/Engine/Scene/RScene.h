
#ifndef ENGINE_SCENE_SCENE_H
#define ENGINE_SCENE_SCENE_H

#include "SceneNode.h"
#include "../Core/Camera.h"
#include "../RHI/RHICommandList.h"
#include "RActor.h"

class RScene;
typedef std::shared_ptr<RScene> RScene_ptr;

class RScene
{
public:
	Camera_ptr GetMainCamera() const {
		return mMainaCmera;
	}

	std::function<void(RHICommandList& RHICmdList)> mRenderFunction;

	void AddActor(RActor_ptr actor);
	void RemoveActor(RActor_ptr actor);

protected:
	Camera_ptr mMainaCmera;
	SceneNode_ptr mRootNode;

	std::list<RActor_ptr> mActors;
};

#endif // ENGINE_SCENE_SCENE_H