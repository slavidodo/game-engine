
#ifndef FRAMEWORK_GRAPHICS_MESH_FACTORY_H
#define FRAMEWORK_GRAPHICS_MESH_FACTORY_H

#include <framework/core/mesh.h>

namespace framework {
namespace graphics {

class MeshFactory
{
public:
	framework::core::Mesh_ptr createCube(float edgeLength);
};

} // ns graphics
} // ns framework

#endif