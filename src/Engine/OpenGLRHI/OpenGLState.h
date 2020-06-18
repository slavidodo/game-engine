
#ifndef ENGINE_OPENGLRHI_OPENGLSTATE_H
#define ENGINE_OPENGLRHI_OPENGLSTATE_H

#include "OpenGLResource.h"

#include "../Core/Math/Rect.h"

#define NUM_OPENGL_VERTEX_STREAMS 16

enum
{
	OGL_MAX_UNIFORM_BUFFER_BINDINGS = 12,
	OGL_FIRST_UNIFORM_BUFFER = 0,
	OGL_MAX_COMPUTE_STAGE_UAV_UNITS = 8,
};

#define ZERO_FILLED_DUMMY_UNIFORM_BUFFER_SIZE 65536

struct OpenGLSamplerStateData
{
	// These enum is just used to count the number of members in this struct
	enum EGLSamplerData
	{
		EGLSamplerData_WrapS,
		EGLSamplerData_WrapT,
		EGLSamplerData_WrapR,
		EGLSamplerData_LODBias,
		EGLSamplerData_MagFilter,
		EGLSamplerData_MinFilter,
		EGLSamplerData_MaxAniso,
		EGLSamplerData_CompareMode,
		EGLSamplerData_CompareFunc,
		EGLSamplerData_Num,
	};

	GLint WrapS;
	GLint WrapT;
	GLint WrapR;
	GLint LODBias;
	GLint MagFilter;
	GLint MinFilter;
	GLint MaxAnisotropy;
	GLint CompareMode;
	GLint CompareFunc;

	OpenGLSamplerStateData()
		: WrapS(GL_REPEAT)
		, WrapT(GL_REPEAT)
		, WrapR(GL_REPEAT)
		, LODBias(0)
		, MagFilter(GL_NEAREST)
		, MinFilter(GL_NEAREST)
		, MaxAnisotropy(1)
		, CompareMode(GL_NONE)
		, CompareFunc(GL_ALWAYS)
	{
	}
};

class OpenGLSamplerState : public RHISamplerState
{
public:
	GLuint Resource;
	OpenGLSamplerStateData Data;

	~OpenGLSamplerState() {
		glDeleteSamplers(1, &Resource);
	}
};

struct OpenGLRasterizerStateData
{
	GLenum FillMode;
	GLenum CullMode;
	float DepthBias;
	float SlopeScaleDepthBias;

	OpenGLRasterizerStateData()
		: FillMode(GL_FILL)
		, CullMode(GL_NONE)
		, DepthBias(0.0f)
		, SlopeScaleDepthBias(0.0f)
	{
	}
};

struct TextureStage
{
	class OpenGLTextureBase* Texture;
	class OpenGLShaderResourceView* SRV;
	GLenum Target;
	GLuint Resource;
	int32_t LimitMip;
	bool bHasMips;
	int32_t NumMips;

	TextureStage()
		: Texture(nullptr)
		, SRV(nullptr)
		, Target(GL_NONE)
		, Resource(0)
		, LimitMip(-1)
		, bHasMips(false)
		, NumMips(0)
	{
	}
};

struct UAVStage
{
	GLenum Format;
	GLuint Resource;

	UAVStage()
		: Format(GL_NONE)
		, Resource(0)
	{
	}
};

#define OpenGLCachedAttr_Invalid (void*)(uintptr_t)0xFFFFFFFF
#define OpenGLCachedAttr_SingleVertex (void*)(uintptr_t)0xFFFFFFFE

struct OpenGLCachedAttr
{
	void* Pointer;
	GLsizei Stride;
	GLuint Buffer;
	GLuint Size;
	GLuint Divisor;
	GLenum Type;
	GLuint StreamOffset;
	GLuint StreamIndex;
	GLboolean Normalized;

	OpenGLCachedAttr() :
		Pointer(OpenGLCachedAttr_Invalid),
		Buffer(0),
		Size(0),
		Stride(-1),
		Divisor(0xFFFFFFFF),
		Type(0),
		StreamOffset(0),
		StreamIndex(0xFFFFFFFF),
		Normalized(GL_FALSE)
	{
	}
};

struct OpenGLStream
{
	OpenGLStream()
		: VertexBuffer(nullptr)
		, Stride(0)
		, MemberOffset(0)
		, Divisor(0)
	{}
	TRefCountPtr<OpenGLVertexBuffer> VertexBuffer;
	uint32_t Stride;
	uint32_t MemberOffset;
	uint32_t Divisor;
};

struct OpenGLCommonState
{
public:
	TextureStage* Textures;
	OpenGLSamplerState** SamplerStates;
	UAVStage* UAVs;

	OpenGLCommonState()
		: Textures(nullptr)
		, SamplerStates(nullptr)
		, UAVs(nullptr)
	{}

	virtual void InitializeResources(int32_t NumCombinedTextures, int32_t NumComputeUAVUnits)
	{
		Textures = new TextureStage[NumCombinedTextures];
		SamplerStates = new OpenGLSamplerState*[NumCombinedTextures];
		UAVs = new UAVStage[NumCombinedTextures];
		memset(SamplerStates, 0, NumCombinedTextures * sizeof(*SamplerStates));
	}

	virtual void CleanupResources()
	{
		delete[] UAVs;
		delete[] SamplerStates;
		delete[] Textures;

		UAVs = NULL;
		SamplerStates = NULL;
		Textures = NULL;
	}
};

// specialized
struct OpenGLContextState : OpenGLCommonState
{
	OpenGLContextState()
		: Framebuffer(0)
		, Program(0)
		, Viewport(0, 0, 0, 0)
		, DepthMinZ(0)
		, DepthMaxZ(0)
		, Scissor(0, 0, 0, 0)
		, ScissorEnabled(false)
		, ArrayBufferBound(0)
		, ElementArrayBufferBound(0)
		, UniformBufferBound(0)
		, ClearColor(0, 0, 0, 0)
		, ClearStencil(0)
		, ClearDepth(0)
		, VertexAttrsEnabledBits(0)
	{
		memset(UniformBuffers, 0, sizeof(UniformBuffers));
		memset(UniformBufferOffsets, 0, sizeof(UniformBufferOffsets));
	}

	OpenGLRasterizerStateData RasterizerState;
	GLuint Framebuffer;
	GLuint Program;
	IntRect Viewport;
	IntRect Scissor;
	bool ScissorEnabled;
	float DepthMinZ;
	float DepthMaxZ;
	GLuint ArrayBufferBound;
	GLuint ElementArrayBufferBound;
	GLuint UniformBufferBound;
	glm::fvec4 ClearColor;
	uint16_t ClearStencil;
	float ClearDepth;

	OpenGLCachedAttr VertexAttrs[NUM_OPENGL_VERTEX_STREAMS];
	OpenGLStream VertexStreams[NUM_OPENGL_VERTEX_STREAMS];
	GLuint UniformBuffers[NUM_SHADER_STAGES * OGL_MAX_UNIFORM_BUFFER_BINDINGS];
	GLuint UniformBufferOffsets[NUM_SHADER_STAGES * OGL_MAX_UNIFORM_BUFFER_BINDINGS];
	std::vector<OpenGLSamplerState*> CachedSamplerStates;

	uint32_t VertexAttrsEnabledBits;
	inline bool GetVertexAttrEnabled(int32_t Index) const
	{
		return !!(VertexAttrsEnabledBits & (1 << Index));
	}
	inline void SetVertexAttrEnabled(int32_t Index, bool enable)
	{
		if (enable) {
			VertexAttrsEnabledBits |= (1 << Index);
		} else {
			VertexAttrsEnabledBits &= ~(1 << Index);
		}
	}

	virtual void InitializeResources(int32_t NumCombinedTextures, int32_t NumComputeUAVUnits) override
	{
		OpenGLCommonState::InitializeResources(NumCombinedTextures, NumComputeUAVUnits);
		CachedSamplerStates.resize(NumCombinedTextures, nullptr);
	}

	virtual void CleanupResources() override
	{
		CachedSamplerStates.clear();
		OpenGLCommonState::CleanupResources();
	}
};

struct OpenGLRHIState : OpenGLCommonState
{
	enum {
		MAX_UNIFORM_BUFFERS_PER_SHADER_STAGE = 14
	};

	OpenGLRasterizerStateData RasterizerState;
	GLuint Framebuffer;
	GLuint ZeroFilledDummyUniformBuffer;
	IntRect Viewport;
	IntRect Scissor;
	float DepthMinZ;
	float DepthMaxZ;
	bool ScissorEnabled;

	OpenGLShaderParameterCache* ShaderParameters;

	TRefCountPtr<OpenGLBoundShaderState> BoundShaderState;
	OpenGLLinkedProgram* LinkedProgramAndDirtyFlag;

	OpenGLStream Streams[NUM_OPENGL_VERTEX_STREAMS];
	uint16_t DirtyUniformBuffers[ShaderFrequency::SF_NumStandardFrequencies];
	RHIUniformBufferRef BoundUniformBuffers[SF_NumStandardFrequencies][MAX_UNIFORM_BUFFERS_PER_SHADER_STAGE];
	bool AnyDirtyUniformBuffers;

	OpenGLRHIState()
		: Framebuffer(0)
		, ZeroFilledDummyUniformBuffer(0)
		, Viewport(0, 0, 0, 0)
		, Scissor(0, 0, 0, 0)
		, DepthMinZ(0)
		, DepthMaxZ(0)
		, ScissorEnabled(false)
		, ShaderParameters(nullptr)
		, BoundShaderState(nullptr)
		, LinkedProgramAndDirtyFlag(nullptr)
		, AnyDirtyUniformBuffers(false)
	{
		memset(DirtyUniformBuffers, 0, sizeof(DirtyUniformBuffers));
		memset(BoundUniformBuffers, 0, sizeof(BoundUniformBuffers));
	}

	virtual void InitializeResources(int32_t NumCombinedTextures, int32_t NumComputeUAVUnits) override;
	virtual void CleanupResources() override;
};

#endif // ENGINE_OPENGLRHI_OPENGLSTATE_H