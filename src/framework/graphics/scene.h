
#ifndef FRAMEWORK_GRAPHICS_SCENE_H
#define FRAMEWORK_GRAPHICS_SCENE_H

#include <framework/core/scene/object.h>
#include <framework/core/scene/scene_node.h>
#include <framework/graphics/camera.h>

namespace framework {
namespace graphics {

class Scene;
typedef std::shared_ptr<Scene> Scene_ptr;

class Scene
{
	using SceneNode_ptr = framework::core::scene::SceneNode_ptr;
	using Object_ptr = framework::core::scene::Object_ptr;

public:
	void render();

private:
	Camera_ptr m_mainCamera;
	SceneNode_ptr m_rootNode;
};

} // ns graphics
} // ns framework

#endif