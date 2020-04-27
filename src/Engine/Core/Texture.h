
#ifndef ENGINE_CORE_IMAGE_H
#define ENGINE_CORE_IMAGE_H

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

struct TextureSettings
{
	textureType_t type;
	textureWrapMode_t wrapMode;
	textureFilterMode_t filterMode;
};

class Texture
{
protected:
	bool Load(uint8_t* buffer, size_t size);

private:
	TextureSettings mTextureSettings;
};

#endif // ENGINE_CORE_IMAGE_H