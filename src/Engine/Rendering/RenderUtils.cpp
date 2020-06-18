
#include "Pch.h"

#include "RenderUtils.h"

#include "../Math/MathUtility.h"

PixelFormatInfo	GPixelFormats[PF_MAX] =
{
	// Name						BlockSizeX	BlockSizeY	BlockSizeZ	BlockBytes	NumComponents	PlatformFormat	Supported		UnrealFormat

	{ "unknown",				0,			0,			0,			0,			0,				0,				0,				PF_Unknown			},
	{ "A32B32G32R32F",			1,			1,			1,			16,			4,				0,				1,				PF_A32B32G32R32F	},
	{ "B8G8R8A8",				1,			1,			1,			4,			4,				0,				1,				PF_B8G8R8A8			},
	{ "G8",						1,			1,			1,			1,			1,				0,				1,				PF_G8				},
	{ "G16",					1,			1,			1,			2,			1,				0,				1,				PF_G16				},
	{ "DXT1",					4,			4,			1,			8,			3,				0,				1,				PF_DXT1				},
	{ "DXT3",					4,			4,			1,			16,			4,				0,				1,				PF_DXT3				},
	{ "DXT5",					4,			4,			1,			16,			4,				0,				1,				PF_DXT5				},
	{ "UYVY",					2,			1,			1,			4,			4,				0,				0,				PF_UYVY				},
	{ "FloatRGB",				1,			1,			1,			4,			3,				0,				1,				PF_FloatRGB			},
	{ "FloatRGBA",				1,			1,			1,			8,			4,				0,				1,				PF_FloatRGBA		},
	{ "DepthStencil",			1,			1,			1,			4,			1,				0,				0,				PF_DepthStencil		},
	{ "ShadowDepth",			1,			1,			1,			4,			1,				0,				0,				PF_ShadowDepth		},
	{ "R32_FLOAT",				1,			1,			1,			4,			1,				0,				1,				PF_R32_FLOAT		},
	{ "G16R16",					1,			1,			1,			4,			2,				0,				1,				PF_G16R16			},
	{ "G16R16F",				1,			1,			1,			4,			2,				0,				1,				PF_G16R16F			},
	{ "G16R16F_FILTER",			1,			1,			1,			4,			2,				0,				1,				PF_G16R16F_FILTER	},
	{ "G32R32F",				1,			1,			1,			8,			2,				0,				1,				PF_G32R32F			},
	{ "A2B10G10R10",			1,          1,          1,          4,          4,              0,              1,				PF_A2B10G10R10		},
	{ "A16B16G16R16",			1,			1,			1,			8,			4,				0,				1,				PF_A16B16G16R16		},
	{ "D24",					1,			1,			1,			4,			1,				0,				1,				PF_D24				},
	{ "PF_R16F",				1,			1,			1,			2,			1,				0,				1,				PF_R16F				},
	{ "PF_R16F_FILTER",			1,			1,			1,			2,			1,				0,				1,				PF_R16F_FILTER		},
	{ "BC5",					4,			4,			1,			16,			2,				0,				1,				PF_BC5				},
	{ "V8U8",					1,			1,			1,			2,			2,				0,				1,				PF_V8U8				},
	{ "A1",						1,			1,			1,			1,			1,				0,				0,				PF_A1				},
	{ "FloatR11G11B10",			1,			1,			1,			4,			3,				0,				0,				PF_FloatR11G11B10	},
	{ "A8",						1,			1,			1,			1,			1,				0,				1,				PF_A8				},
	{ "R32_UINT",				1,			1,			1,			4,			1,				0,				1,				PF_R32_UINT			},
	{ "R32_SINT",				1,			1,			1,			4,			1,				0,				1,				PF_R32_SINT			},

	// IOS Support
	{ "PVRTC2",					8,			4,			1,			8,			4,				0,				0,				PF_PVRTC2			},
	{ "PVRTC4",					4,			4,			1,			8,			4,				0,				0,				PF_PVRTC4			},

	{ "R16_UINT",				1,			1,			1,			2,			1,				0,				1,				PF_R16_UINT			},
	{ "R16_SINT",				1,			1,			1,			2,			1,				0,				1,				PF_R16_SINT			},
	{ "R16G16B16A16_UINT",		1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_UINT},
	{ "R16G16B16A16_SINT",		1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_SINT},
	{ "R5G6B5_UNORM",			1,          1,          1,          2,          3,              0,              1,              PF_R5G6B5_UNORM		},
	{ "R8G8B8A8",				1,			1,			1,			4,			4,				0,				1,				PF_R8G8B8A8			},
	{ "A8R8G8B8",				1,			1,			1,			4,			4,				0,				1,				PF_A8R8G8B8			},
	{ "BC4",					4,			4,			1,			8,			1,				0,				1,				PF_BC4				},
	{ "R8G8",					1,			1,			1,			2,			2,				0,				1,				PF_R8G8				},

	{ "ATC_RGB",				4,			4,			1,			8,			3,				0,				0,				PF_ATC_RGB			},
	{ "ATC_RGBA_E",				4,			4,			1,			16,			4,				0,				0,				PF_ATC_RGBA_E		},
	{ "ATC_RGBA_I",				4,			4,			1,			16,			4,				0,				0,				PF_ATC_RGBA_I		},
	{ "X24_G8",					1,			1,			1,			1,			1,				0,				0,				PF_X24_G8			},
	{ "ETC1",					4,			4,			1,			8,			3,				0,				0,				PF_ETC1				},
	{ "ETC2_RGB",				4,			4,			1,			8,			3,				0,				0,				PF_ETC2_RGB			},
	{ "ETC2_RGBA",				4,			4,			1,			16,			4,				0,				0,				PF_ETC2_RGBA		},
	{ "PF_R32G32B32A32_UINT",	1,			1,			1,			16,			4,				0,				1,				PF_R32G32B32A32_UINT},
	{ "PF_R16G16_UINT",			1,			1,			1,			4,			4,				0,				1,				PF_R16G16_UINT},

	// ASTC support
	{ "ASTC_4x4",				4,			4,			1,			16,			4,				0,				0,				PF_ASTC_4x4			},
	{ "ASTC_6x6",				6,			6,			1,			16,			4,				0,				0,				PF_ASTC_6x6			},
	{ "ASTC_8x8",				8,			8,			1,			16,			4,				0,				0,				PF_ASTC_8x8			},
	{ "ASTC_10x10",				10,			10,			1,			16,			4,				0,				0,				PF_ASTC_10x10		},
	{ "ASTC_12x12",				12,			12,			1,			16,			4,				0,				0,				PF_ASTC_12x12		},

	{ "BC6H",					4,			4,			1,			16,			3,				0,				1,				PF_BC6H				},
	{ "BC7",					4,			4,			1,			16,			4,				0,				1,				PF_BC7				},
	{ "R8_UINT",				1,			1,			1,			1,			1,				0,				1,				PF_R8_UINT			},
	{ "L8",						1,			1,			1,			1,			1,				0,				0,				PF_L8				},
	{ "XGXR8",					1,			1,			1,			4,			4,				0,				1,				PF_XGXR8 			},
	{ "R8G8B8A8_UINT",			1,			1,			1,			4,			4,				0,				1,				PF_R8G8B8A8_UINT	},
	{ "R8G8B8A8_SNORM",			1,			1,			1,			4,			4,				0,				1,				PF_R8G8B8A8_SNORM	},

	{ "R16G16B16A16_UINT",		1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_UNORM },
	{ "R16G16B16A16_SINT",		1,			1,			1,			8,			4,				0,				1,				PF_R16G16B16A16_SNORM },
	{ "PLATFORM_HDR_0",			0,			0,			0,			0,			0,				0,				0,				PF_PLATFORM_HDR_0   },
	{ "PLATFORM_HDR_1",			0,			0,			0,			0,			0,				0,				0,				PF_PLATFORM_HDR_1   },
	{ "PLATFORM_HDR_2",			0,			0,			0,			0,			0,				0,				0,				PF_PLATFORM_HDR_2   },

	// NV12 contains 2 textures: R8 luminance plane followed by R8G8 1/4 size chrominance plane.
	// BlockSize/BlockBytes/NumComponents values don't make much sense for this format, so set them all to one.
	{ "NV12",					1,			1,			1,			1,			1,				0,				0,				PF_NV12             },

	{ "PF_R32G32_UINT",			1,   		1,			1,			8,			2,				0,				1,				PF_R32G32_UINT      },
};

size_t CalcTextureMipWidthInBlocks(uint32_t TextureSizeX, EPixelFormat Format, uint32_t MipIndex)
{
	const uint32_t BlockSizeX = GPixelFormats[Format].BlockSizeX;
	const uint32_t WidthInTexels = Math::Max<uint32_t>(TextureSizeX >> MipIndex, 1);
	const uint32_t WidthInBlocks = (WidthInTexels + BlockSizeX - 1) / BlockSizeX;
	return WidthInBlocks;
}

size_t CalcTextureMipHeightInBlocks(uint32_t TextureSizeY, EPixelFormat Format, uint32_t MipIndex)
{
	const uint32_t BlockSizeY = GPixelFormats[Format].BlockSizeY;
	const uint32_t HeightInTexels = Math::Max<uint32_t>(TextureSizeY >> MipIndex, 1);
	const uint32_t HeightInBlocks = (HeightInTexels + BlockSizeY - 1) / BlockSizeY;
	return HeightInBlocks;
}

size_t CalcTextureMipMapSize(uint32_t TextureSizeX, uint32_t TextureSizeY, EPixelFormat Format, uint32_t MipIndex)
{
	const uint32_t WidthInBlocks = CalcTextureMipWidthInBlocks(TextureSizeX, Format, MipIndex);
	const uint32_t HeightInBlocks = CalcTextureMipHeightInBlocks(TextureSizeY, Format, MipIndex);
	return WidthInBlocks * HeightInBlocks * GPixelFormats[Format].BlockBytes;
}

size_t CalcTextureSize(uint32_t SizeX, uint32_t SizeY, EPixelFormat Format, uint32_t MipCount)
{
	size_t Size = 0;
	for (uint32_t MipIndex = 0; MipIndex < MipCount; ++MipIndex) {
		Size += CalcTextureMipMapSize(SizeX, SizeY, Format, MipIndex);
	}
	return Size;
}

void CalcMipMapExtent3D(uint32_t TextureSizeX, uint32_t TextureSizeY, uint32_t TextureSizeZ, EPixelFormat Format, uint32_t MipIndex, uint32_t& OutXExtent, uint32_t& OutYExtent, uint32_t& OutZExtent)
{
	OutXExtent = Math::Max<uint32_t>(TextureSizeX >> MipIndex, GPixelFormats[Format].BlockSizeX);
	OutYExtent = Math::Max<uint32_t>(TextureSizeY >> MipIndex, GPixelFormats[Format].BlockSizeY);
	OutZExtent = Math::Max<uint32_t>(TextureSizeZ >> MipIndex, GPixelFormats[Format].BlockSizeZ);
}

size_t CalcTextureMipMapSize3D(uint32_t TextureSizeX, uint32_t TextureSizeY, uint32_t TextureSizeZ, EPixelFormat Format, uint32_t MipIndex)
{
	uint32_t XExtent;
	uint32_t YExtent;
	uint32_t ZExtent;
	CalcMipMapExtent3D(TextureSizeX, TextureSizeY, TextureSizeZ, Format, MipIndex, XExtent, YExtent, ZExtent);

	// Offset MipExtent to round up result
	XExtent += GPixelFormats[Format].BlockSizeX - 1;
	YExtent += GPixelFormats[Format].BlockSizeY - 1;
	ZExtent += GPixelFormats[Format].BlockSizeZ - 1;

	const uint32_t XPitch = (XExtent / GPixelFormats[Format].BlockSizeX) * GPixelFormats[Format].BlockBytes;
	const uint32_t NumRows = YExtent / GPixelFormats[Format].BlockSizeY;
	const uint32_t NumLayers = ZExtent / GPixelFormats[Format].BlockSizeZ;

	return NumLayers * NumRows * XPitch;
}

size_t CalcTextureSize3D(uint32_t SizeX, uint32_t SizeY, uint32_t SizeZ, EPixelFormat Format, uint32_t MipCount)
{
	size_t Size = 0;
	for (uint32_t MipIndex = 0; MipIndex < MipCount; ++MipIndex) {
		Size += CalcTextureMipMapSize3D(SizeX, SizeY, SizeZ, Format, MipIndex);
	}
	return Size;
}