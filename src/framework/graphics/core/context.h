
#ifndef FRAMEWORK_GRAPHICS_CORE_CONTEXT_H
#define FRAMEWORK_GRAPHICS_CORE_CONTEXT_H

#include <framework/core/mesh.h>
#include <framework/core/texture.h>
#include <framework/core/material.h>
#include <framework/graphics/camera.h>

namespace framework {
namespace graphics {
namespace core {

class Context
{
public:
	virtual void vsync(bool value) = 0;
	virtual bool vsync() = 0;
	virtual bool vsync() const = 0;

	// TODO; separate those in their own classes
	virtual Shader_ptr makeShader(std::string vertexCode, std::string pixelCode) = 0;
	virtual framework::core::Texture_ptr makeTexture(uint8_t* buffer, uint64_t size) = 0;

	virtual void init() = 0;

	virtual void clear(bool color, bool depth) = 0;
	virtual void clearColor(glm::fvec4 color) = 0;
	virtual void draw(framework::core::Mesh_ptr mesh, framework::core::Material_ptr material, Camera_ptr camera) = 0;
	virtual void resize(glm::uvec2 size) = 0;

	virtual void beginFrame() = 0;
	virtual void endFrame() = 0;
private:

};

} // ns core
} // ns graphics
} // ns framework
#endif
