
#include <framework/pch.h>

#include "opengl_context.h"
#include "opengl_texture.h"

using namespace framework::graphics::opengl;

bool OpenGLTexture::load(unsigned char* buffer, unsigned long long size)
{
	int width = 0, height = 0, numOfChannels;
    unsigned char* data = stbi_load_from_memory(buffer, size, &width, &height, &numOfChannels, 4);
    if (buffer == nullptr)
        return false;

    GLenum format = NULL;
    {
        if (numOfChannels == 1)      format = GL_RED;
        else if (numOfChannels == 3) format = GL_RGB;
        else if (numOfChannels == 4) format = GL_RGBA;
    }

    GLenum target = NULL;
    {
        if (m_settings.type == framework::core::textureType_t::TEXTURE_TYPE_2D)
            target = GL_TEXTURE_2D;
        if (m_settings.type == framework::core::textureType_t::TEXTURE_TYPE_3D)
            target = GL_TEXTURE_3D;
        if (m_settings.type == framework::core::textureType_t::TEXTURE_TYPE_CUBE)
            target = GL_TEXTURE_CUBE_MAP;
    }

    glTexImage2D(target, NULL, format, width, height, NULL, format, GL_UNSIGNED_BYTE, data);
    delete data;

	return true;
}