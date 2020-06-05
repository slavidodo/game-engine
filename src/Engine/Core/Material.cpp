
#include "pch.h"

#include "Material.h"

namespace {

template <typename ValueType>
void TypedValueUniformAssign(Material::TypedValuesMap& map, std::string uniform, Material::MaterialTypedValueType type, ValueType* src, size_t size)
{
	Material::TypedValuesMapIterator it;
	if ((it = map.find(uniform)) != map.end()) {
		Material::MaterialTypedValue& mtv = (*it).second;
		if (mtv.type != type) {
			delete[] mtv.value;
			mtv.type = type;
			mtv.value = (void*)new ValueType[size];
		}

		std::memcpy(mtv.value, (void*)src, size * sizeof(ValueType));
	} else {
		void* value = (void*)new ValueType[size];
		std::memcpy(value, (void*)src, size * sizeof(ValueType));
		map.emplace(std::piecewise_construct, std::forward_as_tuple(uniform),
					std::forward_as_tuple(type, value, size));
	}
}

}

Material::~Material()
{
	// delete resources of typed names
}

void Material::SetBool(std::string uniform, bool value)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_BOOL1, &value, 1);
}

void Material::SetBool(std::string uniform, glm::bvec1 value)
{
	SetBool(uniform, value.x);
}

void Material::SetBool2(std::string uniform, glm::bvec2 value)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_BOOL2 , &value[0], 1);
}

void Material::SetBool3(std::string uniform, glm::bvec3 value)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_BOOL3 , &value[0], 3);
}

void Material::SetBool4(std::string uniform, glm::bvec4 value)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_BOOL4 , &value[0], 4);
}

void Material::SetFloat(std::string uniform, float value)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_FLOAT1 , &value, 1);
}

void Material::SetFloat(std::string uniform, glm::fvec1 value)
{
	SetFloat(uniform, value.x);
}

void Material::SetFloat2(std::string uniform, glm::fvec2 value)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_FLOAT2 , &value[0], 2);
}

void Material::SetFloat3(std::string uniform, glm::fvec3 value)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_FLOAT3 , &value[0], 3);
}

void Material::SetFloat4(std::string uniform, glm::fvec4 value)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_FLOAT4 , &value[0], 4);
}

void Material::SetInt(std::string uniform, int32_t value)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_INT1 , &value, 1);
}

void Material::SetInt(std::string uniform, glm::ivec1 value)
{
	SetInt(uniform, value.x);
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_INT1 , &value[0], 1);
}

void Material::SetInt2(std::string uniform, glm::ivec2 value)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_INT2 , &value[0], 3);
}

void Material::SetInt3(std::string uniform, glm::ivec3 value)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_INT3 , &value[0], 3);
}

void Material::SetInt4(std::string uniform, glm::ivec4 value)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_INT4, &value[0], 4);
}

void Material::SetFloatArray(std::string uniform, float* value, size_t size)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_FLOAT_ARRAY, value, size);
}

void Material::SetFloatArray(std::string uniform, glm::fvec1* value, size_t size)
{
	SetFloatArray(uniform, (float*)value, size);
}

void Material::SetFloat2Array(std::string uniform, glm::fvec2* value, size_t size)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_FLOAT_ARRAY, (float*)value, size * 2);
}

void Material::SetFloat3Array(std::string uniform, glm::fvec3* value, size_t size)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_FLOAT_ARRAY, (float*)value, size * 3);
}

void Material::SetFloat4Array(std::string uniform, glm::fvec4* value, size_t size)
{
	TypedValueUniformAssign(mTypedValues, uniform, MTV_TYPE_FLOAT_ARRAY, (float*)value, size * 4);
}

void Material::ShaderPass() const
{
	TypedValuesMapCIterator it = mTypedValues.begin();
	TypedValuesMapCIterator end = mTypedValues.end();

	for (; it != end; it++) {
		const std::string& uname = (*it).first;
		const void* value = (*it).second.value;
		size_t size = (*it).second.size;

		switch ((*it).second.type) {
			case MTV_TYPE_BOOL1:
				mShader->SetBool(uname, glm::bvec1(((bool*)value)[0]));
				break;
			case MTV_TYPE_BOOL2:
				mShader->SetBool2(uname, glm::make_vec2((bool*)value));
				break;
			case MTV_TYPE_BOOL3:
				mShader->SetBool3(uname, glm::make_vec3((bool*)value));
				break;
			case MTV_TYPE_BOOL4:
				mShader->SetBool4(uname, glm::make_vec4((bool*)value));
				break;
			case MTV_TYPE_FLOAT1:
				mShader->SetFloat(uname, glm::fvec1(((float*)value)[0]));
				break;
			case MTV_TYPE_FLOAT2:
				mShader->SetFloat2(uname, glm::make_vec2((float*)value));
				break;
			case MTV_TYPE_FLOAT3:
				mShader->SetFloat3(uname, glm::make_vec3((float*)value));
				break;
			case MTV_TYPE_FLOAT4:
				mShader->SetFloat4(uname, glm::make_vec4((float*)value));
				break;
			case MTV_TYPE_INT1:
				mShader->SetInt(uname, glm::ivec1(((int*)value)[0]));
				break;
			case MTV_TYPE_INT2:
				mShader->SetInt2(uname, glm::make_vec2((int*)value));
				break;
			case MTV_TYPE_INT3:
				mShader->SetInt3(uname, glm::make_vec3((int*)value));
				break;
			case MTV_TYPE_INT4:
				mShader->SetInt4(uname, glm::make_vec4((int*)value));
				break;
			case MTV_TYPE_MAT3x3:
				mShader->SetMat3x3(uname, glm::make_mat3x3((float*)value));
				break;
			case MTV_TYPE_MAT4x4:
				mShader->SetMat4x4(uname, glm::make_mat4x4((float*)value));
				break;
			case MTV_TYPE_BOOL_ARRAY:
				// TODO;
				break;
			case MTV_TYPE_FLOAT_ARRAY:
				mShader->SetFloatArray(uname, (glm::fvec1*)value, size);
				break;
			default:
				break;
		}
	}
}