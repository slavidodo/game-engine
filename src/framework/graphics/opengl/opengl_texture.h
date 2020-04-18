
#ifndef FRAMEWORK_GRAPHICS_OPENGL_TEXTURE_H
#define FRAMEWORK_GRAPHICS_OPENGL_TEXTURE_H

#include <framework/core/texture.h>

namespace framework {
namespace graphics {
namespace opengl {

class OpenGLTexture : public framework::core::Texture
{
public:
	OpenGLTexture(unsigned char* buffer) {
		load(buffer);
	}

private:
	bool load(unsigned char* buffer) override final;
};

} // ns opengl
} // ns graphics
} // ns framework

#endif
