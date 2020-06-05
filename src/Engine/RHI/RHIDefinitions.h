
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
class RHITexture2D;
class RHITexture3D;
class RHIVertexDeclaration;

typedef TRefCountPtr<RHIVertexBuffer> RHIVertexBufferRef;
typedef TRefCountPtr<RHIIndexBuffer> RHIIndexBufferRef;
typedef TRefCountPtr<RHIUniformBuffer> RHIUniformBufferRef;
typedef TRefCountPtr<RHIVertexShader> RHIVertexShaderRef;
typedef TRefCountPtr<RHIPixelShader> RHIPixelShaderRef;
typedef TRefCountPtr<RHIBoundShaderState> RHIBoundShaderStateRef;
typedef TRefCountPtr<RHITexture2D> RHITexture2DRef;
typedef TRefCountPtr<RHITexture3D> RHITexture3DRef;
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

// structs
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
