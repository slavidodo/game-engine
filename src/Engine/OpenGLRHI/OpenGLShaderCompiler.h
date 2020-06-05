
#ifndef ENGINE_OPENGLRHI_OPENGLSHADERCOMPILER_H
#define ENGINE_OPENGLRHI_OPENGLSHADERCOMPILER_H

#include "../RHI/RHICommandList.h"

class OpenGLShaderCompiler
{
public:
	static RHIVertexShaderRef ManuallyCreateVertexShader(RHICommandList& RHICmdList, std::vector<uint8_t> InCode, uint8_t NumUniformBuffers);
	static RHIPixelShaderRef ManuallyCreatePixelShader(RHICommandList& RHICmdList, std::vector<uint8_t> InCode, uint8_t NumUniformBuffers);
};

#endif // ENGINE_OPENGLRHI_OPENGLSHADERCOMPILER_H
