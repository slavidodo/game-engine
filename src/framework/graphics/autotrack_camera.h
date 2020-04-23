
#ifndef FRAMEWORK_GRAPHICS_AUTOTRACK_CAMERA_H
#define FRAMEWORK_GRAPHICS_AUTOTRACK_CAMERA_H

#include "camera.h"
#include <framework/core/scene/scene_node.h>

namespace framework {
namespace graphics {

class AutoTrackCamera : public Camera
{
public:

private:
	core::scene::SceneNode_ptr m_target;
};

} // ns graphics
} // ns framework

#endif