
#ifndef ENGINE_RENDERING_RENDERUTILS_H
#define ENGINE_RENDERING_RENDERUTILS_H

#include "../RHI/RHIDefinitions.h"

struct PixelFormatInfo
{
	const char* Name;
	int32_t	BlockSizeX,
			BlockSizeY,
			BlockSizeZ,
			BlockBytes,
			NumComponents;
	/** Platform specific token, e.g. D3DFORMAT with D3DDrv										*/
	uint32_t PlatformFormat;
	/** Whether the texture format is supported on the current platform/ rendering combination	*/
	bool Supported;
	EPixelFormat EngineFormat;
};

extern PixelFormatInfo GPixelFormats[PF_MAX];		// Maps members of EPixelFormat to a FPixelFormatInfo describing the format.

size_t CalcTextureMipWidthInBlocks(uint32_t TextureSizeX, EPixelFormat Format, uint32_t MipIndex);

size_t CalcTextureMipHeightInBlocks(uint32_t TextureSizeY, EPixelFormat Format, uint32_t MipIndex);

size_t CalcTextureMipMapSize(uint32_t TextureSizeX, uint32_t TextureSizeY, EPixelFormat Format, uint32_t MipIndex);

size_t CalcTextureSize(uint32_t SizeX, uint32_t SizeY, EPixelFormat Format, uint32_t MipCount);

void CalcMipMapExtent3D(uint32_t TextureSizeX, uint32_t TextureSizeY, uint32_t TextureSizeZ, EPixelFormat Format, uint32_t MipIndex, uint32_t& OutXExtent, uint32_t& OutYExtent, uint32_t& OutZExtent);

size_t CalcTextureMipMapSize3D(uint32_t TextureSizeX, uint32_t TextureSizeY, uint32_t TextureSizeZ, EPixelFormat Format, uint32_t MipIndex);

size_t CalcTextureSize3D(uint32_t SizeX, uint32_t SizeY, uint32_t SizeZ, EPixelFormat Format, uint32_t MipCount);

#endif // ENGINE_RENDERING_RENDERUTILS_H