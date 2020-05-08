
#ifndef ENGINE_OPENGLRHI_OPENGLSTATE_H
#define ENGINE_OPENGLRHI_OPENGLSTATE_H

#include "OpenGLResource.h"

#include "../Core/Math/Rect.h"

#define NUM_OPENGL_VERTEX_STREAMS 16

#define OpenGLCachedAttr_Invalid (void*)(uint32_t)0xFFFFFFFF
#define OpenGLCachedAttr_SingleVertex (void*)(uint32_t)0xFFFFFFFE

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
		Stride(-1),
		Divisor(0xFFFFFFFF),
		Type(0),
		StreamIndex(0xFFFFFFFF)
	{
	}
};

struct OpenGLStream
{
	OpenGLStream()
		: VertexBuffer(0)
		, Stride(0)
		, Offset(0)
		, Divisor(0)
	{}
	OpenGLVertexBuffer_ptr VertexBuffer;
	uint32_t Stride;
	uint32_t Offset;
	uint32_t Divisor;
};

struct OpenGLCommonState
{
public:
private:
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
		, ClearColor(0, 0, 0, 0)
		, ClearStencil(0)
		, ClearDepth(0)
		, VertexAttrsEnabledBits(0) {}

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
	glm::fvec4 ClearColor;
	uint16_t ClearStencil;
	float ClearDepth;

	OpenGLCachedAttr VertexAttrs[NUM_OPENGL_VERTEX_STREAMS];
	OpenGLStream VertexStreams[NUM_OPENGL_VERTEX_STREAMS];

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
};

struct OpenGLRHIState : OpenGLCommonState
{
	OpenGLRasterizerStateData RasterizerState;
	GLuint Framebuffer;
	IntRect Viewport;
	IntRect Scissor;
	float DepthMinZ;
	float DepthMaxZ;
	bool ScissorEnabled;

	OpenGLBoundShaderState_ptr BoundShaderState;
	OpenGLStream Streams[NUM_OPENGL_VERTEX_STREAMS];

	OpenGLRHIState()
		: Framebuffer(0)
		, Viewport(0, 0, 0, 0)
		, Scissor(0, 0, 0, 0)
		, DepthMinZ(0)
		, DepthMaxZ(0)
		, ScissorEnabled(false)
		, BoundShaderState(nullptr) {}
};

#endif // ENGINE_OPENGLRHI_OPENGLSTATE_H