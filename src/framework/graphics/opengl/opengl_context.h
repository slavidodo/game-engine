
#ifndef FRAMEWORK_GRAPHICS_OPENGL_CONTEXT_H
#define FRAMEWORK_GRAPHICS_OPENGL_CONTEXT_H

#include <framework/graphics/core/context.h>
#include <framework/graphics/opengl/mesh_descriptor.h>
#include <framework/graphics/opengl/opengl_shader.h>
#include <framework/graphics/opengl/opengl_texture.h>
#include <SDL2/SDL_video.h>

namespace framework {
namespace graphics {
namespace opengl {

class OpenGLContext : public core::Context
{
public:
	OpenGLContext();
	~OpenGLContext();

	OBJECT_GETACCESSOR(SDL_GLContext, const SDL_GLContext, sdlContext);
	OBJECT_ACCESSOR_DYNAMIC(bool, bool, vsync, SDL_GL_SetSwapInterval, SDL_GL_GetSwapInterval);

	framework::graphics::core::Shader_ptr makeShader(std::string vertexCode, std::string pixelCode) override final {
		return std::make_shared<OpenGLShader>(vertexCode, pixelCode);
	}

	framework::core::BaseMeshDescriptor_ptr makeDescriptor(std::vector<framework::core::VertexDesc>& descs) override final {
		return std::make_shared<MeshDescriptor>(descs);
	}

	framework::core::Texture_ptr makeTexture(unsigned char* buffer) override final {
		return std::make_shared<OpenGLTexture>(buffer);
	}

	void clear(bool color, bool depth) override final;
	void clearColor(glm::fvec4 color) override final;
	void draw(framework::core::BaseMesh_ptr mesh,
			  framework::graphics::core::Shader_ptr shader,
			  glm::mat4 mvp) override final;

	void resize(glm::uvec2 size) override final;

	void beginFrame() override final;
	void endFrame() override final;

	static uint32_t WindowFlags() {
		return SDL_WINDOW_OPENGL;
	}
private:
	SDL_GLContext m_sdlContext;
};

} // ns opengl
} // ns graphics
} // ns framework

#endif
