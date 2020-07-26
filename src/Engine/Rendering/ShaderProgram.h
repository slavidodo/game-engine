#pragma once

#include <fstream>
#include <sstream>


class ShaderProgram {
public:
	/// Shader program functions
	ShaderProgram() {}
	ShaderProgram(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath = nullptr);
	ShaderProgram(std::string vShaderCode, std::string fShaderCode);
	void Activate() const;
	GLuint GetID() const;

	/// Uniform functions
	void SetBoolUniform(const std::string& name, bool value) const;
	void SetIntUniform(const std::string& name, int value) const;
	void SetFloatUniform(const std::string& name, float value) const;
	void SetFloatMatrix3Uniform(const std::string& name, glm::mat3 value) const;
	void SetFloatMatrix4Uniform(const std::string& name, glm::mat4 value) const;
	void SetFloatVector3Uniform(const std::string& name, glm::vec3 value) const;
	void BindUniformBlock(const std::string& name, GLuint bindingPoint) const;

private:
	GLuint m_id;

	/// Shader program functions
	void InitializeShaderProgram(const GLuint& vShaderID, const GLuint& fShaderID, const GLuint& gShaderID);
	
	/// Shader functions
	unsigned int InitializeShader(const char* shaderPath, GLenum shaderType);
	const std::string* ReadFile(const char* shaderPath);
	void ShaderCleanup(const GLuint& vShaderID, const GLuint& fShaderID, const GLuint& gShaderID);
};