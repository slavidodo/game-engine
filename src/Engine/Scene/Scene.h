
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

private:
	Camera_ptr m_mainCamera;
	SceneNode_ptr m_rootNode;
};

#endif // ENGINE_SCENE_SCENE_H