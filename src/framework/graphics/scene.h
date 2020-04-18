
#ifndef FRAMEWORK_GRAPHICS_SCENE_H
#define FRAMEWORK_GRAPHICS_SCENE_H

#include <framework/graphics/camera.h>

namespace framework {
namespace graphics {

class Scene
{
public:

private:
	std::shared_ptr<Camera> m_mainCamera;
};

} // ns graphics
} // ns framework

#endif