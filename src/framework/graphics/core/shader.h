
#ifndef FRAMEWORK_GRAPHICS_CORE_SHADER_H
#define FRAMEWORK_GRAPHICS_CORE_SHADER_H

namespace framework {
namespace graphics {
namespace core {

class Shader;

typedef std::shared_ptr<Shader> Shader_ptr;

class Shader
{
public:
	virtual void setBool(std::string global, glm::bvec1 value) = 0;
	virtual void setBool2(std::string global, glm::bvec2 value) = 0;
	virtual void setBool3(std::string global, glm::bvec3 value) = 0;
	virtual void setBool4(std::string global, glm::bvec4 value) = 0;

	virtual void setInt(std::string global, glm::ivec1 value) = 0;
	virtual void setInt2(std::string global, glm::ivec2 value) = 0;
	virtual void setInt3(std::string global, glm::ivec3 value) = 0;
	virtual void setInt4(std::string global, glm::ivec4 value) = 0;

	virtual void setFloat(std::string global, glm::fvec1 value) = 0;
	virtual void setFloat2(std::string global, glm::fvec2 value) = 0;
	virtual void setFloat3(std::string global, glm::fvec3 value) = 0;
	virtual void setFloat4(std::string global, glm::fvec4 value) = 0;

	// common attributes
	virtual void setFloatArray(std::string global, glm::fvec1* array, std::size_t size) = 0;
	virtual void setFloat2Array(std::string global, glm::fvec2* array, std::size_t size) = 0;
	virtual void setFloat3Array(std::string global, glm::fvec3* array, std::size_t size) = 0;
	virtual void setFloat4Array(std::string global, glm::fvec4* array, std::size_t size) = 0;

	virtual void setMat4x4(std::string global, glm::fmat4 mat) = 0;

	virtual void use() = 0;
};

} // ns core
} // ns graphics
} // ns framework

#endif