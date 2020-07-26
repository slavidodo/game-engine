#include "pch.h"
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* vShaderPath, const char* fShaderPath, const char* gShaderPath) {
	try {
		GLuint vShaderID, fShaderID, gShaderID;
		vShaderID = InitializeShader(vShaderPath, GL_VERTEX_SHADER);
		fShaderID = InitializeShader(fShaderPath, GL_FRAGMENT_SHADER);
		if (gShaderPath != nullptr) gShaderID = InitializeShader(gShaderPath, GL_GEOMETRY_SHADER);
		
		InitializeShaderProgram(vShaderID, fShaderID, gShaderID);
		ShaderCleanup(vShaderID, fShaderID, gShaderID);
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_READ_FAILURE" << std::endl;
	}
}
ShaderProgram::ShaderProgram(std::string vShaderCode, std::string fShaderCode) {
	GLuint vShaderID;
	{
		vShaderID = glCreateShader(GL_VERTEX_SHADER);
		const char* shaderSourceCode = vShaderCode.c_str();
		glShaderSource(vShaderID, 1, &shaderSourceCode, NULL);
		glCompileShader(vShaderID);

		int compileStatus;
		glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE) {
			char log[512];
			glGetShaderInfoLog(vShaderID, 512, NULL, log);
			std::cout << "ERROR::VSHADER::COMPILATION" << std::endl << log;
		}
	}

	GLuint fShaderID;
	{
		fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		const char* shaderSourceCode = fShaderCode.c_str();
		glShaderSource(fShaderID, 1, &shaderSourceCode, NULL);
		glCompileShader(fShaderID);

		int compileStatus;
		glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE) {
			char log[512];
			glGetShaderInfoLog(fShaderID, 512, NULL, log);
			std::cout << "ERROR::FSHADER::COMPILATION" << std::endl << log;
		}
	}

	InitializeShaderProgram(vShaderID, fShaderID, 0);
	ShaderCleanup(vShaderID, fShaderID, 0);
}

/// Initialization functions
void ShaderProgram::InitializeShaderProgram(const GLuint& vShaderID, const GLuint& fShaderID, const GLuint& gShaderID) {
	/// Create shader program
	m_id = glCreateProgram();

	/// Attach shaders to program
	{
		glAttachShader(m_id, vShaderID);
		glAttachShader(m_id, fShaderID);
		if (gShaderID != NULL) glAttachShader(m_id, gShaderID);
	}

	/// Link program shaders
	{
		glLinkProgram(m_id);
		int linkStatus;
		glGetProgramiv(m_id, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE) {
			char log[512];
			glGetProgramInfoLog(m_id, 512, NULL, log);
			std::cout << "ERROR::SHADER_PROGRAM::LINKING\n" << log;
		}
	}
}
unsigned int ShaderProgram::InitializeShader(const char* shaderPath, GLenum shaderType) {
	/// Create shader object
	unsigned int shaderID;
	shaderID = glCreateShader(shaderType);

	/// Copy source into shader
	{
		const std::string* src = ReadFile(shaderPath);
		const char* shaderSourceCode = src->c_str();
		glShaderSource(shaderID, 1, &shaderSourceCode, NULL);
		delete src;
	}

	/// Compile shader
	{
		glCompileShader(shaderID);
		int compileStatus;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE) {
			char log[512];
			glGetShaderInfoLog(shaderID, 512, NULL, log);
			std::cout << shaderPath << std::endl << "ERROR::SHADER::COMPILATION" << std::endl << log;
			return NULL;
		}
	}

	return shaderID;
}
const std::string* ShaderProgram::ReadFile(const char* shaderPath) {
	std::string* sourceCode = new std::string;

	/// Read source code from file into string stream
	{
		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		shaderFile.open(shaderPath);
		std::stringstream stringStream;
		stringStream << shaderFile.rdbuf();
		shaderFile.close();
		*sourceCode = stringStream.str();
	}
	
	return sourceCode;
}
void ShaderProgram::ShaderCleanup(const GLuint& vShaderID, const GLuint& fShaderID, const GLuint& gShaderID) {
	glDeleteShader(vShaderID);
	glDeleteShader(fShaderID);
	if (gShaderID) glDeleteShader(gShaderID);
}

/// Utility functions
GLuint ShaderProgram::GetID() const {
	return m_id;
}
void ShaderProgram::Activate() const {
	glUseProgram(m_id);
}

/// Uniform functions
void ShaderProgram::SetBoolUniform(const std::string& name, bool value) const {
	GLint temp;
	glGetIntegerv(GL_CURRENT_PROGRAM, &temp);
	glUseProgram(m_id);

	int uniformLocation = glGetUniformLocation(m_id, name.c_str());
	if (uniformLocation != -1) glUniform1i(uniformLocation, (int)value);
	else std::cout << "Cannot find uniform " << name << std::endl;
	
	glUseProgram(temp);
}
void ShaderProgram::SetIntUniform(const std::string& name, int value) const {
	GLint temp;
	glGetIntegerv(GL_CURRENT_PROGRAM, &temp);
	glUseProgram(m_id);
	
	int uniformLocation = glGetUniformLocation(m_id, name.c_str());
	if (uniformLocation != -1) glUniform1i(uniformLocation, value);
	else std::cout << "Cannot find uniform " << name << std::endl;

	glUseProgram(temp);
}
void ShaderProgram::SetFloatUniform(const std::string& name, float value) const {
	GLint temp;
	glGetIntegerv(GL_CURRENT_PROGRAM, &temp);
	glUseProgram(m_id);
	
	int uniformLocation = glGetUniformLocation(m_id, name.c_str());
	if (uniformLocation != -1) glUniform1f(uniformLocation, value);
	else std::cout << "Cannot find uniform " << name << std::endl;

	glUseProgram(temp);
}
void ShaderProgram::SetFloatMatrix3Uniform(const std::string& name, glm::mat3 value) const {
	GLint temp;
	glGetIntegerv(GL_CURRENT_PROGRAM, &temp);
	glUseProgram(m_id);
	
	int uniformLocation = glGetUniformLocation(m_id, name.c_str());
	if (uniformLocation != -1) glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
	else std::cout << "Cannot find uniform " << name << std::endl;

	glUseProgram(temp);
}
void ShaderProgram::SetFloatMatrix4Uniform(const std::string& name, glm::mat4 value) const {
	GLint temp;
	glGetIntegerv(GL_CURRENT_PROGRAM, &temp);
	glUseProgram(m_id);

	int uniformLocation = glGetUniformLocation(m_id, name.c_str());
	if (uniformLocation != -1) glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
	else std::cout << "Cannot find uniform " << name << std::endl;

	glUseProgram(temp);
}
void ShaderProgram::SetFloatVector3Uniform(const std::string& name, glm::vec3 value) const {
	GLint temp;
	glGetIntegerv(GL_CURRENT_PROGRAM, &temp);
	glUseProgram(m_id);
	
	int uniformLocation = glGetUniformLocation(m_id, name.c_str());
	if (uniformLocation != -1) glUniform3f(uniformLocation, value.x, value.y, value.z);
	else std::cout << "Cannot find uniform " << name << std::endl;

	glUseProgram(temp);
}

void ShaderProgram::BindUniformBlock(const std::string& name, GLuint bindingPoint) const {
	GLuint uniformBlockIndex = glGetUniformBlockIndex(m_id, name.c_str());
	if (uniformBlockIndex != GL_INVALID_VALUE) glUniformBlockBinding(m_id, uniformBlockIndex, bindingPoint);
	else std::cout << "ERROR FINDING " << name << " UNIFORM BLOCK\n";
}