
#ifndef ENGINE_OPENGLRHI_OPENGLRTSHADER_H
#define ENGINE_OPENGLRHI_OPENGLRTSHADER_H

#include "../core/shader.h"

class OpenGLRTShader : public Shader
{
public:
	~OpenGLRTShader();

	explicit OpenGLRTShader(std::string vertexSource, std::string pixelSource);
	explicit OpenGLRTShader(std::string vertexSource, std::string pixelSource, std::string geometrySource);

	inline void SetBool(std::string global, glm::bvec1 value) override final;
	inline void SetBool2(std::string global, glm::bvec2 value) override final;
	inline void SetBool3(std::string global, glm::bvec3 value) override final;
	inline void SetBool4(std::string global, glm::bvec4 value) override final;

	inline void SetInt(std::string global, glm::ivec1 value) override final;
	inline void SetInt2(std::string global, glm::ivec2 value) override final;
	inline void SetInt3(std::string global, glm::ivec3 value) override final;
	inline void SetInt4(std::string global, glm::ivec4 value) override final;

	inline void SetFloat(std::string global, glm::fvec1 value) override final;
	inline void SetFloat2(std::string global, glm::fvec2 value) override final;
	inline void SetFloat3(std::string global, glm::fvec3 value) override final;
	inline void SetFloat4(std::string global, glm::fvec4 value) override final;

	// common attributes
	inline void SetFloatArray(std::string global, glm::fvec1* array, std::size_t size) override final;
	inline void SetFloat2Array(std::string global, glm::fvec2* array, std::size_t size) override final;
	inline void SetFloat3Array(std::string global, glm::fvec3* array, std::size_t size) override final;
	inline void SetFloat4Array(std::string global, glm::fvec4* array, std::size_t size) override final;

	inline void SetMat4x4(std::string global, glm::fmat4 mat) override final;

	void Use() override final;

protected:
	OpenGLRTShader() = default;

private:
	uint32_t mProgramId = -1;
	bool mInitialized = false;

	void CheckInit();

	int32_t GetUniformLocation(const std::string& global);
};

#endif // ENGINE_OPENGLRHI_OPENGLRTSHADER_H