
#ifndef FRAMEWORK_GRAPHICS_OPENGL_SHADER_H
#define FRAMEWORK_GRAPHICS_OPENGL_SHADER_H

#include "../core/shader.h"

namespace framework {
namespace graphics {
namespace opengl {

class OpenGLShader : public core::Shader
{
public:
	~OpenGLShader();

	explicit OpenGLShader(std::string vertexSource, std::string pixelSource);
	explicit OpenGLShader(std::string vertexSource, std::string pixelSource, std::string geometrySource);

	inline void setBool(std::string global, glm::bvec1 value) override final;
	inline void setBool2(std::string global, glm::bvec2 value) override final;
	inline void setBool3(std::string global, glm::bvec3 value) override final;
	inline void setBool4(std::string global, glm::bvec4 value) override final;

	inline void setInt(std::string global, glm::ivec1 value) override final;
	inline void setInt2(std::string global, glm::ivec2 value) override final;
	inline void setInt3(std::string global, glm::ivec3 value) override final;
	inline void setInt4(std::string global, glm::ivec4 value) override final;

	inline void setFloat(std::string global, glm::fvec1 value) override final;
	inline void setFloat2(std::string global, glm::fvec2 value) override final;
	inline void setFloat3(std::string global, glm::fvec3 value) override final;
	inline void setFloat4(std::string global, glm::fvec4 value) override final;

	// common attributes
	inline void setFloatArray(std::string global, glm::fvec1* array, std::size_t size) override final;
	inline void setFloat2Array(std::string global, glm::fvec2* array, std::size_t size) override final;
	inline void setFloat3Array(std::string global, glm::fvec3* array, std::size_t size) override final;
	inline void setFloat4Array(std::string global, glm::fvec4* array, std::size_t size) override final;

	inline void setMat4x4(std::string global, glm::mat4x4 mat) override final;

	void use() override final;

protected:
	OpenGLShader() = default;

private:
	uint32_t m_programId = -1;
	bool m_initialized = false;

	void checkInit();

	int32_t getLocation(const std::string& global);
};

} // ns opengl
} // ns graphics
} // ns framework

#endif