
#ifndef FRAMEWORK_GRAPHICS_OPENGL_CONTEXT_H
#define FRAMEWORK_GRAPHICS_OPENGL_CONTEXT_H

#include <framework/graphics/core/context.h>
#include <framework/graphics/opengl/opengl_shader.h>
#include <framework/graphics/opengl/opengl_texture.h>

namespace framework {
namespace graphics {
namespace opengl {

class OpenGLContext : public core::Context
{
public:
	OpenGLContext();

	bool vsync() const { return m_vsync; }
	void vsync(bool value) {
		m_vsync = value;
		glfwSwapInterval(m_vsync);
	}

	Shader_ptr makeShader(std::string vertexCode, std::string pixelCode) override final {
		return std::make_shared<OpenGLShader>(vertexCode, pixelCode);
	}

	Texture_ptr makeTexture(uint8_t* buffer, uint64_t size) override final {
		return std::make_shared<OpenGLTexture>(buffer, size);
	}

	void init() override final;

	void clear(bool color, bool depth) override final;
	void clearColor(glm::fvec4 color) override final;
	void draw(framework::core::Mesh_ptr mesh,
			  framework::core::Material_ptr shader,
			  framework::graphics::Camera_ptr camera) override final;

	void resize(glm::uvec2 size) override final;

	void beginFrame() override final;
	void endFrame() override final;

private:
	bool m_vsync;
};

} // ns opengl
} // ns graphics
} // ns framework

#endif
