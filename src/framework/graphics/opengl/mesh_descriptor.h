
#ifndef FRAMEWORK_GRAPHICS_OPENGL_MESHDESCRIPTOR_H
#define FRAMEWORK_GRAPHICS_OPENGL_MESHDESCRIPTOR_H

#include <framework/core/mesh.h>
#include <framework/graphics/opengl/vertex_descriptor.h>

#include <GL/glew.h>

namespace framework {
namespace graphics {
namespace opengl {

class MeshDescriptor : public framework::core::BaseMeshDescriptor
{
public:
	MeshDescriptor(std::vector<framework::core::VertexDesc> const& descs)
		: BaseMeshDescriptor(descs) {}

	~MeshDescriptor() {
		destroy();
	}

	void update() override final;
	void destroy() override final;

	OBJECT_GETACCESSOR(uint32_t, uint32_t, VAO)

private:
	uint32_t m_VAO = 0;
	uint32_t m_buffers[2] {0};

	bool m_updated = false;
};

} // ns opengl
} // ns graphics
} // ns framework

#endif