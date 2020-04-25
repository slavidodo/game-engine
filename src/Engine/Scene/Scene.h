
#ifndef ENGINE_SCENE_SCENE_H
#define ENGINE_SCENE_SCENE_H

#include "SceneNode.h"
#include "../Core/Camera.h"

class Scene;
typedef std::shared_ptr<Scene> Scene_ptr;

class Scene
{
public:
	void Render();

protected:
	Camera_ptr mMainCamera;
	SceneNode_ptr mRootNode;
};

#endif // ENGINE_SCENE_SCENE_H