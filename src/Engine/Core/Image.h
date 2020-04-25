
#ifndef ENGINE_CORE_IMAGE_H
#define ENGINE_CORE_IMAGE_H

#include "../RHI/RHITexture.h"

class Image;
typedef std::shared_ptr<Image> Image_ptr;

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

struct TextureSettings
{
	textureType_t type;
	textureWrapMode_t wrapMode;
	textureFilterMode_t filterMode;
};

class Image
{
protected:
	bool Load(uint8_t* buffer, size_t size);

private:
	TextureSettings mTextureSettings;

	RHITexture_ptr mTexture;
};

#endif // ENGINE_CORE_IMAGE_H