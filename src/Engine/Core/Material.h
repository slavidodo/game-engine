
#ifndef ENGINE_CORE_MATERIAL_H
#define ENGINE_CORE_MATERIAL_H

#include "Shader.h"

class Material;
typedef std::shared_ptr<Material> Material_ptr;
typedef std::weak_ptr<Material> Material_weakPtr;

class Material
{
public:
	explicit Material(Shader_ptr shader)
		: mShader(shader) {
		assert(shader == nullptr);
	}

	// Set these values on 
	void SetBool(std::string global, bool value);
	void SetBool(std::string global, glm::bvec1 value);
	void SetBool2(std::string global, glm::bvec2 value);
	void SetBool3(std::string global, glm::bvec3 value);
	void SetBool4(std::string global, glm::bvec4 value);

	void SetInt(std::string global, int32_t value);
	void SetInt(std::string global, glm::ivec1 value);
	void SetInt2(std::string global, glm::ivec2 value);
	void SetInt3(std::string global, glm::ivec3 value);
	void SetInt4(std::string global, glm::ivec4 value);

	void SetFloat(std::string global, float value);
	void SetFloat(std::string global, glm::fvec1 value);
	void SetFloat2(std::string global, glm::fvec2 value);
	void SetFloat3(std::string global, glm::fvec3 value);
	void SetFloat4(std::string global, glm::fvec4 value);

	void SetFloatArray(std::string global, glm::fvec1* value, std::size_t size);
	void SetFloat2Array(std::string global, glm::fvec2* value, std::size_t size);
	void SetFloat3Array(std::string global, glm::fvec3* value, std::size_t size);
	void SetFloat4Array(std::string global, glm::fvec4* value, std::size_t size);

protected:
	/*
	 * pass the typed values to the RTShader
	 */
	void ShaderPass() const;

	enum MaterialTypedValueType {
		MTV_TYPE_BOOL1,
		MTV_TYPE_BOOL2,
		MTV_TYPE_BOOL3,
		MTV_TYPE_BOOL4,
		MTV_TYPE_FLOAT1,
		MTV_TYPE_FLOAT2,
		MTV_TYPE_FLOAT3,
		MTV_TYPE_FLOAT4,
		MTV_TYPE_INT1,
		MTV_TYPE_INT2,
		MTV_TYPE_INT3,
		MTV_TYPE_INT4,
		MTV_TYPE_MAT3x3,
		MTV_TYPE_MAT4x3,
		MTV_TYPE_MAT4x4,
		MTV_TYPE_BOOL_ARRAY,
		MTV_TYPE_FLOAT_ARRAY,
		MTV_TYPE_FLOAT4_ARRAY,
	};

	struct MaterialTypedValue {
		void* value; // using unions will use a huge amount of memory for say, a boolean.
		MaterialTypedValueType type;
		size_t size;
	};

	Shader_ptr mShader;
	std::unordered_map<std::string, MaterialTypedValue> mTypedValues;
};

#endif // ENGINE_CORE_MATERIAL_H