
#include "Pch.h"

#include "OpenGLShaderCompiler.h"
#include "OpenGLDynamicRHI.h"

RHIVertexShaderRef OpenGLShaderCompiler::ManuallyCreateVertexShader(RHICommandList& RHICmdList, std::vector<uint8_t> InCode, uint8_t NumUniformBuffers)
{
	RHIVertexShaderRef VertexShaderRHI = RHICmdList.CreateVertexShader(InCode);
	OpenGLVertexShader* VertexShader = StaticResourceCast(VertexShaderRHI.GetReference())->GetGLResourceObject();
	VertexShader->Bindings.NumUniformBuffers = NumUniformBuffers;
	return VertexShaderRHI;
}

RHIPixelShaderRef OpenGLShaderCompiler::ManuallyCreatePixelShader(RHICommandList& RHICmdList, std::vector<uint8_t> InCode, uint8_t NumUniformBuffers)
{
	RHIPixelShaderRef PixelShaderRHI = RHICmdList.CreatePixelShader(InCode);
	OpenGLPixelShader* PixelShader = StaticResourceCast(PixelShaderRHI.GetReference())->GetGLResourceObject();
	PixelShader->Bindings.NumUniformBuffers = NumUniformBuffers;
	return PixelShaderRHI;
}