
#ifndef FRAMEWORK_GRAPHICS_OPENGL_VERTEXDESCRIPTOR_H
#define FRAMEWORK_GRAPHICS_OPENGL_VERTEXDESCRIPTOR_H

#include <framework/core/mesh.h>
#include <GL/glew.h>

namespace framework {
namespace graphics {
namespace opengl {

inline void vertex_descriptor(const std::vector<framework::core::VertexDesc>& descs)
{
	for (size_t i = 0; i < descs.size(); i++) {
		const auto& desc = descs[i];
		// todo GL_FLOAT should be desc.type
		glVertexAttribPointer(i, desc.size, GL_FLOAT, desc.normalized ? GL_TRUE : GL_FALSE, desc.stride, desc.offset);
		glEnableVertexAttribArray(i);
	}
};

} // ns opengl
} // ns graphics
} // ns framework

#endif