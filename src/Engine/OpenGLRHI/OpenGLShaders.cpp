
#include "pch.h"

#include "OpenGLDynamicRHI.h"

namespace {

template<typename OpenGLShaderType>
std::shared_ptr<OpenGLShaderType> CompileOpenGLShader(RHIShader* ShaderRHI, const std::vector<uint8_t>& Code)
{
	// TODO: Check for shader cache

	GLint Resource = glCreateShader(OpenGLShaderType::TypeEnum);

	int32_t length = Code.size();
	const char* code = (const char*)Code.data();
	glShaderSource(Resource, 1, &code, &length);
	glCompileShader(Resource);

	GLint CompileStatus = GL_TRUE;
	glGetShaderiv(Resource, GL_COMPILE_STATUS, &CompileStatus);

	if (!CompileStatus) {
		char infoLog[512];
		glGetShaderInfoLog(Resource, 512, NULL, infoLog);
		std::cout << "Shader (type=" << OpenGLShaderType::TypeEnum << ") Compilation Failed:\n" << infoLog << std::endl;
	} else {
		std::cout << "Shader (type=" << OpenGLShaderType::TypeEnum << ") Compiled Successfully" << std::endl;
	}

	return std::make_shared<OpenGLShaderType>(Resource);
}

template<typename RHIShaderType, typename OpenGLProxyType>
std::shared_ptr<RHIShaderType> CreateProxyShader(const std::vector<uint8_t>& Code)
{
	auto CreateFunc = [&] (RHIShaderType* ShaderRHI) {
		return CompileOpenGLShader<typename OpenGLProxyType::OpenGLType>(ShaderRHI, Code);
	};

	return std::dynamic_pointer_cast<RHIShaderType>(std::make_shared<OpenGLProxyType>(CreateFunc));
}

}

RHIVertexShader_ptr OpenGLDynamicRHI::RHICreateVertexShader(const std::vector<uint8_t>& Code)
{
	return CreateProxyShader<RHIVertexShader, OpenGLVertexShaderProxy>(Code);
}

RHIPixelShader_ptr OpenGLDynamicRHI::RHICreatePixelShader(const std::vector<uint8_t>& Code)
{
	return CreateProxyShader<RHIPixelShader, OpenGLPixelShaderProxy>(Code);
}

RHIBoundShaderState_ptr OpenGLDynamicRHI::RHICreateBoundShaderState(
	RHIVertexDeclaration_ptr VertexDeclarationRHI,
	RHIVertexShader_ptr VertexShaderRHI,
	RHIPixelShader_ptr PixelShaderRHI)
{
	assert(VertexShaderRHI && PixelShaderRHI);

	// TODO: Cache this state, we shouldn't create a shader program
	// everytime for the same combination

	auto VertexShader = std::dynamic_pointer_cast<OpenGLVertexShaderProxy>(VertexShaderRHI)->GetGLResourceObject();
	auto PixelShader = std::dynamic_pointer_cast<OpenGLPixelShaderProxy>(PixelShaderRHI)->GetGLResourceObject();

	// TODO:
	// also we could use a config to handle shaders in a staged manner as
	// their actual representation in the state

	GLuint Program = glCreateProgram();
	glAttachShader(Program, VertexShader->GetGLResource());
	glAttachShader(Program, PixelShader->GetGLResource());
	glLinkProgram(Program);

	// TODO: error check for linking success, compile status, ...
	OpenGLLinkedProgram_ptr LinkedProgram = std::make_shared<OpenGLLinkedProgram>(Program);
	OpenGLVertexDeclaration_ptr VertexDeclaration = std::dynamic_pointer_cast<OpenGLVertexDeclaration>(VertexDeclarationRHI);
	OpenGLBoundShaderState_ptr BoundShaderState = std::make_shared<OpenGLBoundShaderState>(
		LinkedProgram,
		VertexDeclaration,
		VertexShaderRHI,
		PixelShaderRHI
	);

	return BoundShaderState;
}