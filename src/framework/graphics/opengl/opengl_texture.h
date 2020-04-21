
#ifndef FRAMEWORK_GRAPHICS_OPENGL_TEXTURE_H
#define FRAMEWORK_GRAPHICS_OPENGL_TEXTURE_H

#include <framework/core/texture.h>

#include <external/stbi_image.h>

namespace framework {
namespace graphics {
namespace opengl {

class OpenGLTexture : public framework::core::Texture
{
public:
	OpenGLTexture(uint8_t* buffer, uint64_t size) {
		load(buffer, size);
	}

private:
	bool load(uint8_t* buffer, uint64_t size) override final;
};

} // ns opengl
} // ns graphics
} // ns framework

#endif
