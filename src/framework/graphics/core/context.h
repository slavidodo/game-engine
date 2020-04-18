
#ifndef FRAMEWORK_GRAPHICS_CORE_CONTEXT
#define FRAMEWORK_GRAPHICS_CORE_CONTEXT

#include <framework/core/mesh.h>
#include <framework/core/texture.h>
#include <framework/graphics/core/shader.h>

namespace framework {
namespace graphics {
namespace core {

class Context {
public:

	virtual void vsync(bool value) = 0;
	virtual bool vsync() = 0;
	virtual bool vsync() const = 0;

	virtual Shader_ptr makeShader(std::string vertexCode, std::string pixelCode) = 0;
	virtual framework::core::BaseMeshDescriptor_ptr makeDescriptor(std::vector<framework::core::VertexDesc>& descs) = 0;
	virtual framework::core::Texture_ptr makeTexture(unsigned char* buffer, unsigned long long size) = 0;

	template<typename VertexType, typename ...Args>
	framework::core::BaseMesh_ptr makeMesh(Args... args) {
		auto descriptor = makeDescriptor(framework::core::get_vertex_desc<VertexType>());
		auto mesh = std::make_shared<framework::core::Mesh<VertexType>>(std::forward<Args>(args)...);
		mesh->descriptor(descriptor); // stores a shared_ptr to the descriptor to maintain its lifetime
		descriptor->mesh(mesh); // stores a weak_ptr to the mesh
		return mesh;
	}

	virtual void clear(bool color, bool depth) = 0;
	virtual void clearColor(glm::fvec4 color) = 0;
	virtual void draw(framework::core::BaseMesh_ptr mesh, Shader_ptr shader, glm::mat4 mvp) = 0;
	virtual void resize(glm::uvec2 size) = 0;

	virtual void beginFrame() = 0;
	virtual void endFrame() = 0;

	virtual void drawRotatingCube();

private:

};

} // ns core
} // ns graphics
} // ns framework
#endif
