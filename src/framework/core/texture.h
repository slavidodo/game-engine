
#ifndef FRAMEWORK_CORE_TEXTURE_H
#define FRAMEWORK_CORE_TEXTURE_H

namespace framework {
namespace core {

class Texture;
typedef std::shared_ptr<Texture> Texture_ptr;

enum textureType_t
{
	TEXTURE_TYPE_2D,
	TEXTURE_TYPE_3D,
	TEXTURE_TYPE_CUBE
};

enum textureWrapMode_t
{
	TEXTURE_WRAPMODE_CLAMP,
	TEXTURE_WRAPMODE_REPEAT,
	TEXTURE_WRAPMODE_MIRROR,
};

enum textureFilterMode_t
{
	TEXTURE_FILTERMODE_POINT,
	TEXTURE_FILTERMODE_BILINEAR,
	TEXTURE_FILTERMODE_TRILINEAR,
};

struct TextureSetting
{
	textureType_t type;
	textureWrapMode_t wrapMode;
	textureFilterMode_t filterMode;
};

class Texture
{
private:
	virtual bool load(unsigned char* buffer) = 0;

	TextureSetting m_settings; // 2d/3d/cube/wrap/compression
};

} // ns core
} // ns framework

#endif