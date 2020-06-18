
#ifndef ENGINE_RHI_RHIDEFINITIONS_H
#define ENGINE_RHI_RHIDEFINITIONS_H

#include "../Atomic/ThreadSafeRefCountedObject.h"
#include "../Atomic/TRefCountPtr.h"

/** Alignment of the shader parameters struct is required to be 16-byte boundaries. */
#define SHADER_PARAMETER_STRUCT_ALIGNMENT 16

#define SHADER_PARAMETER_POINTER_ALIGNMENT sizeof(uint64_t)

/** The alignment in bytes between elements of array shader parameters. */
#define SHADER_PARAMETER_ARRAY_ELEMENT_ALIGNMENT 16

class RHIResource;
class RHIVertexBuffer;
class RHIIndexBuffer;
class RHIUniformBuffer;
class RHIVertexShader;
class RHIPixelShader;
class RHIBoundShaderState;
class RHITexture;
class RHITexture2D;
class RHITexture2DArray;
class RHITexture3D;
class RHITextureCube;
class RHITextureReference;
class RHIVertexDeclaration;

typedef TRefCountPtr<RHIVertexBuffer> RHIVertexBufferRef;
typedef TRefCountPtr<RHIIndexBuffer> RHIIndexBufferRef;
typedef TRefCountPtr<RHIUniformBuffer> RHIUniformBufferRef;
typedef TRefCountPtr<RHIVertexShader> RHIVertexShaderRef;
typedef TRefCountPtr<RHIPixelShader> RHIPixelShaderRef;
typedef TRefCountPtr<RHIBoundShaderState> RHIBoundShaderStateRef;
typedef TRefCountPtr<RHITexture2D> RHITexture2DRef;
typedef TRefCountPtr<RHIVertexDeclaration> RHIVertexDeclarationRef;

// enums
enum class RHIResourceLockMode : uint8_t
{
	RLM_NORMAL,
	RLM_READ_ONLY,
	RLM_WRITE_ONLY,
	RLM_WRITE_NO_OVERWRITE,
};

enum RHIHardwareBufferUsage : uint8_t
{
	// we could specify general uses, such as STREAM_DRAW
	HWBU_STATIC = 1 << 0, // to be filled once
	HWBU_DYNAMIC = 1 << 2, // to be modified more often
	HWBU_DISCARD = 1 << 3, // to be streamed every frame

	// other uses with buffer data
	HWBU_WRITE_ONLY = 1 << 4, // contents are not read back
	HWBU_READ_ONLY = 1 << 5, // contents are to be read from the gpu, often RT usage

	HWBU_ZERO_STRIDE = 1 << 6,

	HWBU_STATIC_WRITE_ONLY = HWBU_STATIC | HWBU_WRITE_ONLY, // optimal buffer usage
	HWBU_DYNAMIC_WRITE_ONLY = HWBU_DYNAMIC | HWBU_WRITE_ONLY, // write only dynamic

	HWBU_DYNAMIC_WRITE_ONLY_DISCARD = HWBU_DYNAMIC_WRITE_ONLY | HWBU_DISCARD, // dyanmic, write only discardable buffer for regular refilling
};

enum RHIIndexBufferType {
	IBT_16,
	IBT_32,
};

enum VertexElementType
{
	VET_None,
	VET_Float1,
	VET_Float2,
	VET_Float3,
	VET_Float4,
	VET_PackedNormal,	// FPackedNormal
	VET_UByte4,
	VET_UByte4N,
	VET_Color,
	VET_Short2,
	VET_Short4,
	VET_Short2N,		// 16 bit word normalized to (value/32767.0,value/32767.0,0,0,1)
	VET_Half2,			// 16 bit float using 1 bit sign, 5 bit exponent, 10 bit mantissa 
	VET_Half4,
	VET_Short4N,		// 4 X 16 bit word, normalized 
	VET_UShort2,
	VET_UShort4,
	VET_UShort2N,		// 16 bit word normalized to (value/65535.0,value/65535.0,0,0,1)
	VET_UShort4N,		// 4 X 16 bit word unsigned, normalized 
	VET_URGB10A2N,		// 10 bit r, g, b and 2 bit a normalized to (value/1023.0f, value/1023.0f, value/1023.0f, value/3.0f)
	VET_UInt,
	VET_MAX,
};

enum EUniformBufferBaseType : uint8_t
{
	UBMT_INVALID,

	// Parameter types.
	UBMT_BOOL,
	UBMT_INT32,
	UBMT_UINT32,
	UBMT_FLOAT32,

	// RHI resources not tracked by render graph.
	UBMT_TEXTURE,
	UBMT_SRV,
	UBMT_UAV,
	UBMT_SAMPLER,

	// Resources tracked by render graph.
	UBMT_RDG_TEXTURE,
	UBMT_RDG_TEXTURE_SRV,
	UBMT_RDG_TEXTURE_UAV,
	UBMT_RDG_BUFFER,
	UBMT_RDG_BUFFER_SRV,
	UBMT_RDG_BUFFER_UAV,

	// Nested structure.
	UBMT_NESTED_STRUCT,

	// Structure that is nested on C++ side, but included on shader side.
	UBMT_INCLUDED_STRUCT,

	// GPU Indirection reference of struct, like is currently named Uniform buffer.
	UBMT_REFERENCED_STRUCT,

	// Structure dedicated to setup render targets for a rasterizer pass.
	UBMT_RENDER_TARGET_BINDING_SLOTS,
};

enum EUniformBufferUsage
{
	// the uniform buffer is temporary, used for a single draw call then discarded
	UniformBuffer_SingleDraw = 0,
	// the uniform buffer is used for multiple draw calls but only for the current frame
	UniformBuffer_SingleFrame,
	// the uniform buffer is used for multiple draw calls, possibly across multiple frames
	UniformBuffer_MultiFrame,
};

enum class EUniformBufferValidation
{
	None,
	ValidateResources
};

enum class RasterizerFillMode
{
	FM_Point,
	FM_Wireframe,
	FM_Solid,
};

enum class RasterizerCullMode
{
	CM_None,
	CM_ClockWise,
	CM_CounterClockWise,
};

enum ShaderFrequency
{
	SF_Vertex = 0,
	SF_Hull = 1,
	SF_Domain = 2,
	SF_Pixel = 3,
	SF_Geometry = 4,
	SF_Compute = 5,

	// Number of standard SM5-style graphics pipeline shader frequencies
	SF_NumStandardFrequencies = 6,
};

// TODO: move this to global
enum ShaderStage : uint8_t
{
	SHADER_STAGE_VERTEX = 0,
	SHADER_STAGE_PIXEL,
	SHADER_STAGE_GEOMETRY,
	SHADER_STAGE_HULL,
	SHADER_STAGE_DOMAIN,
	NUM_NON_COMPUTE_SHADER_STAGES,
	SHADER_STAGE_COMPUTE = NUM_NON_COMPUTE_SHADER_STAGES,
	NUM_SHADER_STAGES,
};

enum class PackedTypeName : int8_t
{
	HighP = 'h',	// Make sure these enums match hlslcc
	MediumP = 'm',
	LowP = 'l',
	Int = 'i',
	Uint = 'u',

	Invalid = ' ',
};

enum class PackedTypeIndex : int8_t
{
	HighP = 0,
	MediumP = 1,
	LowP = 2,
	Int = 3,
	Uint = 4,

	Max = 5,
	Invalid = -1,
};

enum
{
	PACKED_TYPENAME_HIGHP = (int32_t)PackedTypeName::HighP,	// Make sure these enums match hlslcc
	PACKED_TYPENAME_MEDIUMP = (int32_t)PackedTypeName::MediumP,
	PACKED_TYPENAME_LOWP = (int32_t)PackedTypeName::LowP,
	PACKED_TYPENAME_INT = (int32_t)PackedTypeName::Int,
	PACKED_TYPENAME_UINT = (int32_t)PackedTypeName::Uint,
	PACKED_TYPENAME_SAMPLER = 's',
	PACKED_TYPENAME_IMAGE = 'g',

	PACKED_TYPEINDEX_HIGHP = (int32_t)PackedTypeIndex::HighP,
	PACKED_TYPEINDEX_MEDIUMP = (int32_t)PackedTypeIndex::MediumP,
	PACKED_TYPEINDEX_LOWP = (int32_t)PackedTypeIndex::LowP,
	PACKED_TYPEINDEX_INT = (int32_t)PackedTypeIndex::Int,
	PACKED_TYPEINDEX_UINT = (int32_t)PackedTypeIndex::Uint,
	PACKED_TYPEINDEX_MAX = (int32_t)PackedTypeIndex::Max,
};

/** Flags used for texture creation */
enum ETextureCreateFlags
{
	TexCreate_None = 0,

	// Texture can be used as a render target
	TexCreate_RenderTargetable = 1 << 0,
	// Texture can be used as a resolve target
	TexCreate_ResolveTargetable = 1 << 1,
	// Texture can be used as a depth-stencil target.
	TexCreate_DepthStencilTargetable = 1 << 2,
	// Texture can be used as a shader resource.
	TexCreate_ShaderResource = 1 << 3,
	// Texture is encoded in sRGB gamma space
	TexCreate_SRGB = 1 << 4,
	// Texture data is writable by the CPU
	TexCreate_CPUWritable = 1 << 5,
	// Texture will be created with an un-tiled format
	TexCreate_NoTiling = 1 << 6,
	// Texture will be used for video decode
	TexCreate_VideoDecode = 1 << 7,
	// Texture that may be updated every frame
	TexCreate_Dynamic = 1 << 8,
	// Texture will be used as a render pass attachment that will be read from
	TexCreate_InputAttachmentRead = 1 << 9,
	// Disable automatic defragmentation if the initial texture memory allocation fails.
	TexCreate_DisableAutoDefrag = 1 << 10,
	// Create the texture with automatic -1..1 biasing
	TexCreate_BiasNormalMap = 1 << 11,
	// Create the texture with the flag that allows mip generation later, only applicable to D3D11
	TexCreate_GenerateMipCapable = 1 << 12,
	// The texture can be partially allocated in fastvram
	TexCreate_FastVRAMPartialAlloc = 1 << 13,
	// Do not create associated shader resource view, only applicable to D3D11 and D3D12
	TexCreate_DisableSRVCreation = 1 << 14,
	// UnorderedAccessView (DX11 only)
	// Warning: Causes additional synchronization between draw calls when using a render target allocated with this flag, use sparingly
	// See: GCNPerformanceTweets.pdf Tip 37
	TexCreate_UAV = 1 << 16,
	// Render target texture that will be displayed on screen (back buffer)
	TexCreate_Presentable = 1 << 17,
	// Texture data is accessible by the CPU
	TexCreate_CPUReadback = 1 << 18,
	// Texture was processed offline (via a texture conversion process for the current platform)
	TexCreate_OfflineProcessed = 1 << 19,
	// Texture needs to go in fast VRAM if available (HINT only)
	TexCreate_FastVRAM = 1 << 20,
	// by default the texture is not showing up in the list - this is to reduce clutter, using the FULL option this can be ignored
	TexCreate_HideInVisualizeTexture = 1 << 21,
	// Texture should be created in virtual memory, with no physical memory allocation made
	// You must make further calls to RHIVirtualTextureSetFirstMipInMemory to allocate physical memory
	// and RHIVirtualTextureSetFirstMipVisible to map the first mip visible to the GPU
	TexCreate_Virtual = 1 << 22,
	// Creates a RenderTargetView for each array slice of the texture
	// Warning: if this was specified when the resource was created, you can't use SV_RenderTargetArrayIndex to route to other slices!
	TexCreate_TargetArraySlicesIndependently = 1 << 23,
	// Texture that may be shared with DX9 or other devices
	TexCreate_Shared = 1 << 24,
	// RenderTarget will not use full-texture fast clear functionality.
	TexCreate_NoFastClear = 1 << 25,
	// Texture is a depth stencil resolve target
	TexCreate_DepthStencilResolveTarget = 1 << 26,
	// Flag used to indicted this texture is a streamable 2D texture, and should be counted towards the texture streaming pool budget.
	TexCreate_Streamable = 1 << 27,
	// Render target will not FinalizeFastClear; Caches and meta data will be flushed, but clearing will be skipped (avoids potentially trashing metadata)
	TexCreate_NoFastClearFinalize = 1 << 28,
	// Hint to the driver that this resource is managed properly by the engine for Alternate-Frame-Rendering in mGPU usage.
	TexCreate_AFRManual = 1 << 29,
	// Workaround for 128^3 volume textures getting bloated 4x due to tiling mode on PS4
	TexCreate_ReduceMemoryWithTilingMode = 1 << 30,
	/** Texture should be allocated from transient memory. */
	TexCreate_Transient = 1 << 31
};

enum EPixelFormat
{
	PF_Unknown = 0,
	PF_A32B32G32R32F = 1,
	PF_B8G8R8A8 = 2,
	PF_G8 = 3,
	PF_G16 = 4,
	PF_DXT1 = 5,
	PF_DXT3 = 6,
	PF_DXT5 = 7,
	PF_UYVY = 8,
	PF_FloatRGB = 9,
	PF_FloatRGBA = 10,
	PF_DepthStencil = 11,
	PF_ShadowDepth = 12,
	PF_R32_FLOAT = 13,
	PF_G16R16 = 14,
	PF_G16R16F = 15,
	PF_G16R16F_FILTER = 16,
	PF_G32R32F = 17,
	PF_A2B10G10R10 = 18,
	PF_A16B16G16R16 = 19,
	PF_D24 = 20,
	PF_R16F = 21,
	PF_R16F_FILTER = 22,
	PF_BC5 = 23,
	PF_V8U8 = 24,
	PF_A1 = 25,
	PF_FloatR11G11B10 = 26,
	PF_A8 = 27,
	PF_R32_UINT = 28,
	PF_R32_SINT = 29,
	PF_PVRTC2 = 30,
	PF_PVRTC4 = 31,
	PF_R16_UINT = 32,
	PF_R16_SINT = 33,
	PF_R16G16B16A16_UINT = 34,
	PF_R16G16B16A16_SINT = 35,
	PF_R5G6B5_UNORM = 36,
	PF_R8G8B8A8 = 37,
	PF_A8R8G8B8 = 38,	// Only used for legacy loading; do NOT use!
	PF_BC4 = 39,
	PF_R8G8 = 40,
	PF_ATC_RGB = 41,
	PF_ATC_RGBA_E = 42,
	PF_ATC_RGBA_I = 43,
	PF_X24_G8 = 44,	// Used for creating SRVs to alias a DepthStencil buffer to read Stencil. Don't use for creating textures.
	PF_ETC1 = 45,
	PF_ETC2_RGB = 46,
	PF_ETC2_RGBA = 47,
	PF_R32G32B32A32_UINT = 48,
	PF_R16G16_UINT = 49,
	PF_ASTC_4x4 = 50,	// 8.00 bpp
	PF_ASTC_6x6 = 51,	// 3.56 bpp
	PF_ASTC_8x8 = 52,	// 2.00 bpp
	PF_ASTC_10x10 = 53,	// 1.28 bpp
	PF_ASTC_12x12 = 54,	// 0.89 bpp
	PF_BC6H = 55,
	PF_BC7 = 56,
	PF_R8_UINT = 57,
	PF_L8 = 58,
	PF_XGXR8 = 59,
	PF_R8G8B8A8_UINT = 60,
	PF_R8G8B8A8_SNORM = 61,
	PF_R16G16B16A16_UNORM = 62,
	PF_R16G16B16A16_SNORM = 63,
	PF_PLATFORM_HDR_0 = 64,
	PF_PLATFORM_HDR_1 = 65,	// Reserved.
	PF_PLATFORM_HDR_2 = 66,	// Reserved.
	PF_NV12 = 67,
	PF_R32G32_UINT = 68,
	PF_MAX = 69,
};

// structs
struct BaseShaderResourceTable
{
	/** Bits indicating which resource tables contain resources bound to this shader. */
	uint32_t ResourceTableBits;

	/** Mapping of bound SRVs to their location in resource tables. */
	std::vector<uint32_t> ShaderResourceViewMap;

	/** Mapping of bound sampler states to their location in resource tables. */
	std::vector<uint32_t> SamplerMap;

	/** Mapping of bound UAVs to their location in resource tables. */
	std::vector<uint32_t> UnorderedAccessViewMap;

	/** Hash of the layouts of resource tables at compile time, used for runtime validation. */
	std::vector<uint32_t> ResourceTableLayoutHashes;

	BaseShaderResourceTable() :
		ResourceTableBits(0)
	{
	}

	bool operator==(const BaseShaderResourceTable& B)
	{
		const BaseShaderResourceTable& A = *this;

		bool Equal = true;
		Equal &= (A.ResourceTableBits == B.ResourceTableBits);
		Equal &= (A.ShaderResourceViewMap.size() == B.ShaderResourceViewMap.size());
		Equal &= (A.SamplerMap.size() == B.SamplerMap.size());
		Equal &= (A.UnorderedAccessViewMap.size() == B.UnorderedAccessViewMap.size());
		Equal &= (A.ResourceTableLayoutHashes.size() == B.ResourceTableLayoutHashes.size());
		if (!Equal) {
			return false;
		}
		Equal &= (memcmp(A.ShaderResourceViewMap.data(), B.ShaderResourceViewMap.data(), sizeof(uint32_t) * A.ShaderResourceViewMap.size()) == 0);
		Equal &= (memcmp(A.SamplerMap.data(), B.SamplerMap.data(), sizeof(uint32_t) * A.SamplerMap.size()) == 0);
		Equal &= (memcmp(A.UnorderedAccessViewMap.data(), B.UnorderedAccessViewMap.data(), sizeof(uint32_t) * A.UnorderedAccessViewMap.size()) == 0);
		Equal &= (memcmp(A.ResourceTableLayoutHashes.data(), B.ResourceTableLayoutHashes.data(), sizeof(uint32_t) * A.ResourceTableLayoutHashes.size()) == 0);
		return Equal;
	}
};

struct UniformBufferCopyInfo
{
	uint16_t SourceOffsetInFloats;
	uint8_t SourceUBIndex;
	uint8_t DestUBIndex;
	uint8_t DestUBTypeName;
	uint8_t DestUBTypeIndex;
	uint16_t DestOffsetInFloats;
	uint16_t SizeInFloats;
};

struct PackedArrayInfo
{
	uint16_t Size;
	uint8_t TypeName;
	uint8_t TypeIndex;
};

typedef std::vector<PackedArrayInfo> PackedArrayInfoVec;

// functions
static inline uint8_t ShaderStageIndexToTypeName(uint8_t Stage)
{
	switch (Stage) {
		case SHADER_STAGE_VERTEX:	return 'v';
		case SHADER_STAGE_PIXEL:	return 'p';
		case SHADER_STAGE_GEOMETRY:	return 'g';
		case SHADER_STAGE_HULL:		return 'h';
		case SHADER_STAGE_DOMAIN:	return 'd';
		case SHADER_STAGE_COMPUTE:	return 'c';
		default: break;
	}
	return 0;
}

static inline uint8_t PackedTypeIndexToTypeName(uint8_t ArrayType)
{
	switch (ArrayType) {
		case PACKED_TYPEINDEX_HIGHP:	return PACKED_TYPENAME_HIGHP;
		case PACKED_TYPEINDEX_MEDIUMP:	return PACKED_TYPENAME_MEDIUMP;
		case PACKED_TYPEINDEX_LOWP:		return PACKED_TYPENAME_LOWP;
		case PACKED_TYPEINDEX_INT:		return PACKED_TYPENAME_INT;
		case PACKED_TYPEINDEX_UINT:		return PACKED_TYPENAME_UINT;
		default: break;
	}
	return 0;
}

static inline uint8_t PackedTypeNameToTypeIndex(uint8_t ArrayName)
{
	switch (ArrayName) {
		case PACKED_TYPENAME_HIGHP:		return PACKED_TYPEINDEX_HIGHP;
		case PACKED_TYPENAME_MEDIUMP:	return PACKED_TYPEINDEX_MEDIUMP;
		case PACKED_TYPENAME_LOWP:		return PACKED_TYPEINDEX_LOWP;
		case PACKED_TYPENAME_INT:		return PACKED_TYPEINDEX_INT;
		case PACKED_TYPENAME_UINT:		return PACKED_TYPEINDEX_UINT;
		default: break;
	}
	return 0;
}

#endif // ENGINE_RHI_RHIDEFINITIONS_H
