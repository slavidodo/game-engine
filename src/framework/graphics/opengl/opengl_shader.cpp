
#include "pch.h"

#include "opengl_shader.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

using namespace framework::graphics::opengl;

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

OpenGLShader::OpenGLShader(std::string vertexSource, std::string pixelSource)
{
	checkInit();

	int32_t vertexShader = createOGLShader(m_programId, GL_VERTEX_SHADER, std::move(vertexSource));
	int32_t pixelShader = createOGLShader(m_programId, GL_FRAGMENT_SHADER, std::move(pixelSource));

	glLinkProgram(m_programId);
	glDeleteShader(vertexShader);
	glDeleteShader(pixelShader);
}

OpenGLShader::OpenGLShader(std::string vertexSource, std::string pixelSource, std::string geometrySource)
{
	checkInit();

	int32_t vertexShader = createOGLShader(m_programId, GL_VERTEX_SHADER, std::move(vertexSource));
	int32_t pixelShader = createOGLShader(m_programId, GL_FRAGMENT_SHADER, std::move(pixelSource));
	int32_t geometryShader = createOGLShader(m_programId, GL_GEOMETRY_SHADER, std::move(geometrySource));

	glLinkProgram(m_programId);
	glDeleteShader(vertexShader);
	glDeleteShader(pixelShader);
	glDeleteShader(geometryShader);
}

OpenGLShader::~OpenGLShader()
{
	if (m_initialized) {
		glDeleteProgram(m_programId);
	}
}

void OpenGLShader::checkInit()
{
	if (!m_initialized) {
		m_programId = glCreateProgram();
		m_initialized = true;
	}
}

int32_t OpenGLShader::getLocation(const std::string& global)
{
	return glGetUniformLocation(m_programId, global.c_str());
}

void OpenGLShader::setBool(std::string global, glm::bvec1 value)
{
	checkInit();
	glUniform1i(glGetUniformLocation(m_programId, global.c_str()), (int32_t)value[0]);
}

void OpenGLShader::setBool2(std::string global, glm::bvec2 value)
{
	checkInit();
	glUniform2iv(glGetUniformLocation(m_programId, global.c_str()), 1, glm::value_ptr(glm::ivec2(value)));
}

void OpenGLShader::setBool3(std::string global, glm::bvec3 value)
{
	checkInit();
	glUniform3iv(glGetUniformLocation(m_programId, global.c_str()), 1, glm::value_ptr(glm::ivec3(value)));
}

void OpenGLShader::setBool4(std::string global, glm::bvec4 value)
{
	checkInit();
	glUniform4iv(glGetUniformLocation(m_programId, global.c_str()), 1, glm::value_ptr(glm::ivec4(value)));
}

void OpenGLShader::setInt(std::string global, glm::ivec1 value)
{
	checkInit();
	glUniform1iv(glGetUniformLocation(m_programId, global.c_str()), 1, &value[0]);
}

void OpenGLShader::setInt2(std::string global, glm::ivec2 value)
{
	checkInit();
	glUniform2iv(glGetUniformLocation(m_programId, global.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::setInt3(std::string global, glm::ivec3 value)
{
	checkInit();
	glUniform3iv(glGetUniformLocation(m_programId, global.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::setInt4(std::string global, glm::ivec4 value)
{
	checkInit();
	glUniform4iv(glGetUniformLocation(m_programId, global.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::setFloat(std::string global, glm::fvec1 value)
{
	checkInit();
	glUniform1fv(glGetUniformLocation(m_programId, global.c_str()), 1, &value[0]);
}

void OpenGLShader::setFloat2(std::string global, glm::fvec2 value)
{
	checkInit();
	glUniform2fv(glGetUniformLocation(m_programId, global.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::setFloat3(std::string global, glm::fvec3 value)
{
	checkInit();
	glUniform3fv(glGetUniformLocation(m_programId, global.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::setFloat4(std::string global, glm::fvec4 value)
{
	checkInit();
	glUniform4fv(glGetUniformLocation(m_programId, global.c_str()), 1, glm::value_ptr(value));
}

void OpenGLShader::setFloatArray(std::string global, glm::fvec1* array, std::size_t size)
{
	checkInit();
	glUniform1fv(getLocation(global), (int32_t)size, &(array[0])[0]);
}

void OpenGLShader::setFloat2Array(std::string global, glm::fvec2* array, std::size_t size)
{
	checkInit();
	glUniform2fv(getLocation(global), (int32_t)size, glm::value_ptr(array[0]));
}

void OpenGLShader::setFloat3Array(std::string global, glm::fvec3* array, std::size_t size)
{
	checkInit();
	glUniform3fv(getLocation(global), (int32_t)size, glm::value_ptr(array[0]));
}

void OpenGLShader::setFloat4Array(std::string global, glm::fvec4* array, std::size_t size)
{
	checkInit();
	glUniform4fv(getLocation(global), (int32_t)size, glm::value_ptr(array[0]));
}

void OpenGLShader::setMat4x4(std::string global, glm::fmat4 mat)
{
	checkInit();
	glUniformMatrix4fv(getLocation(global), 1, GL_FALSE, glm::value_ptr(mat));
}

void OpenGLShader::use()
{
	glUseProgram(m_programId);
}