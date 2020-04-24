
#ifndef ENGINE_CORE_SHADER_H
#define ENGINE_CORE_SHADER_H

class Shader;
typedef std::shared_ptr<Shader> Shader_ptr;

class Shader
{
public:
	virtual void SetBool(std::string global, glm::bvec1 value) = 0;
	virtual void SetBool2(std::string global, glm::bvec2 value) = 0;
	virtual void SetBool3(std::string global, glm::bvec3 value) = 0;
	virtual void SetBool4(std::string global, glm::bvec4 value) = 0;

	virtual void SetInt(std::string global, glm::ivec1 value) = 0;
	virtual void SetInt2(std::string global, glm::ivec2 value) = 0;
	virtual void SetInt3(std::string global, glm::ivec3 value) = 0;
	virtual void SetInt4(std::string global, glm::ivec4 value) = 0;

	virtual void SetFloat(std::string global, glm::fvec1 value) = 0;
	virtual void SetFloat2(std::string global, glm::fvec2 value) = 0;
	virtual void SetFloat3(std::string global, glm::fvec3 value) = 0;
	virtual void SetFloat4(std::string global, glm::fvec4 value) = 0;

	// common attributes
	virtual void SetFloatArray(std::string global, glm::fvec1* array, std::size_t size) = 0;
	virtual void SetFloat2Array(std::string global, glm::fvec2* array, std::size_t size) = 0;
	virtual void SetFloat3Array(std::string global, glm::fvec3* array, std::size_t size) = 0;
	virtual void SetFloat4Array(std::string global, glm::fvec4* array, std::size_t size) = 0;

	virtual void SetMat3x3(std::string global, glm::fmat3x3 mat) = 0;
	virtual void SetMat4x4(std::string global, glm::fmat4x4 mat) = 0;

	virtual void Use() = 0;
};

#endif // ENGINE_CORE_SHADER_H