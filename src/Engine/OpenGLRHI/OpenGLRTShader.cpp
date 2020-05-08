
#include "pch.h"

#include "OpenGLRTShader.h"

namespace {

int32_t createOGLShader(int32_t programId, int32_t type, const std::string& source) {
	int32_t oglShader = glCreateShader(type);
	const char* shaderCode = source.c_str();
	glShaderSource(oglShader, 1, &shaderCode, nullptr);
	glCompileShader(oglShader);
	glAttachShader(programId, oglShader);
	return oglShader;
}

}

OpenGLRTShader::OpenGLRTShader(std::string vertexSource, std::string pixelSource)
{
	CheckInit();

	int32_t vertexShader = createOGLShader(mProgramId, GL_VERTEX_SHADER, std::move(vertexSource));
	int32_t pixelShader = createOGLShader(mProgramId, GL_FRAGMENT_SHADER, std::move(pixelSource));

	glLinkProgram(mProgramId);
	glDeleteShader(vertexShader);
	glDeleteShader(pixelShader);
}

OpenGLRTShader::OpenGLRTShader(std::string vertexSource, std::string pixelSource, std::string geometrySource)
{
	CheckInit();

	int32_t vertexShader = createOGLShader(mProgramId, GL_VERTEX_SHADER, std::move(vertexSource));
	int32_t pixelShader = createOGLShader(mProgramId, GL_FRAGMENT_SHADER, std::move(pixelSource));
	int32_t geometryShader = createOGLShader(mProgramId, GL_GEOMETRY_SHADER, std::move(geometrySource));

	glLinkProgram(mProgramId);
	glDeleteShader(vertexShader);
	glDeleteShader(pixelShader);
	glDeleteShader(geometryShader);
}

OpenGLRTShader::~OpenGLRTShader()
{
	if (mInitialized) {
		glDeleteProgram(mProgramId);
	}
}

void OpenGLRTShader::CheckInit()
{
	if (!mInitialized) {
		mProgramId = glCreateProgram();
		mInitialized = true;
	}
}

int32_t OpenGLRTShader::GetUniformLocation(const std::string& global)
{
	return glGetUniformLocation(mProgramId, global.c_str());
}

void OpenGLRTShader::SetBool(std::string global, glm::bvec1 value)
{
	CheckInit();
	glUniform1i(GetUniformLocation(global), (int32_t)value[0]);
}

void OpenGLRTShader::SetBool2(std::string global, glm::bvec2 value)
{
	CheckInit();
	glUniform2iv(GetUniformLocation(global), 1, glm::value_ptr(glm::ivec2(value)));
}

void OpenGLRTShader::SetBool3(std::string global, glm::bvec3 value)
{
	CheckInit();
	glUniform3iv(GetUniformLocation(global), 1, glm::value_ptr(glm::ivec3(value)));
}

void OpenGLRTShader::SetBool4(std::string global, glm::bvec4 value)
{
	CheckInit();
	glUniform4iv(GetUniformLocation(global), 1, glm::value_ptr(glm::ivec4(value)));
}

void OpenGLRTShader::SetInt(std::string global, glm::ivec1 value)
{
	CheckInit();
	glUniform1iv(GetUniformLocation(global), 1, &value[0]);
}

void OpenGLRTShader::SetInt2(std::string global, glm::ivec2 value)
{
	CheckInit();
	glUniform2iv(GetUniformLocation(global), 1, glm::value_ptr(value));
}

void OpenGLRTShader::SetInt3(std::string global, glm::ivec3 value)
{
	CheckInit();
	glUniform3iv(GetUniformLocation(global), 1, glm::value_ptr(value));
}

void OpenGLRTShader::SetInt4(std::string global, glm::ivec4 value)
{
	CheckInit();
	glUniform4iv(GetUniformLocation(global), 1, glm::value_ptr(value));
}

void OpenGLRTShader::SetFloat(std::string global, glm::fvec1 value)
{
	CheckInit();
	glUniform1fv(GetUniformLocation(global), 1, &value[0]);
}

void OpenGLRTShader::SetFloat2(std::string global, glm::fvec2 value)
{
	CheckInit();
	glUniform2fv(GetUniformLocation(global), 1, glm::value_ptr(value));
}

void OpenGLRTShader::SetFloat3(std::string global, glm::fvec3 value)
{
	CheckInit();
	glUniform3fv(GetUniformLocation(global), 1, glm::value_ptr(value));
}

void OpenGLRTShader::SetFloat4(std::string global, glm::fvec4 value)
{
	CheckInit();
	glUniform4fv(GetUniformLocation(global), 1, glm::value_ptr(value));
}

void OpenGLRTShader::SetFloatArray(std::string global, glm::fvec1* array, std::size_t size)
{
	CheckInit();
	glUniform1fv(GetUniformLocation(global), (int32_t)size, &(array[0])[0]);
}

void OpenGLRTShader::SetFloat2Array(std::string global, glm::fvec2* array, std::size_t size)
{
	CheckInit();
	glUniform2fv(GetUniformLocation(global), (int32_t)size, glm::value_ptr(array[0]));
}

void OpenGLRTShader::SetFloat3Array(std::string global, glm::fvec3* array, std::size_t size)
{
	CheckInit();
	glUniform3fv(GetUniformLocation(global), (int32_t)size, glm::value_ptr(array[0]));
}

void OpenGLRTShader::SetFloat4Array(std::string global, glm::fvec4* array, std::size_t size)
{
	CheckInit();
	glUniform4fv(GetUniformLocation(global), (int32_t)size, glm::value_ptr(array[0]));
}

void OpenGLRTShader::SetMat4x4(std::string global, glm::fmat4 mat)
{
	CheckInit();
	glUniformMatrix4fv(GetUniformLocation(global), 1, GL_FALSE, glm::value_ptr(mat));
}

void OpenGLRTShader::Use()
{
	glUseProgram(mProgramId);
}