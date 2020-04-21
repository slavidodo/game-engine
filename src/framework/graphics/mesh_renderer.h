
#ifndef FRAMEWORK_GRAPHICS_MESHRENDERER_H
#define FRAMEWORK_GRAPHICS_MESHRENDERER_H

#include <framework/core/mesh.h>
#include <framework/core/material.h>

namespace framework {
namespace graphics {

class MeshRenderer
{
	using Mesh_ptr = framework::core::Mesh_ptr;
	using Material_ptr = framework::core::Material_ptr;

public:
	// issues a draw call with nesecarry hardware buffers to draw a mesh
	void render();

private:
	Mesh_ptr m_mesh;
	Material_ptr m_material;
};

} // ns graphics
} // ns framework

#endif