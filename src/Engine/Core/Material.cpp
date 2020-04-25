
#include "pch.h"

#include "Material.h"

void Material::ShaderPass() const
{
	auto it = mTypedValues.begin();
	auto end = mTypedValues.end();
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
			case MTV_TYPE_FLOAT4_ARRAY:
				mShader->SetFloat4Array(uname, (glm::fvec4*)value, size);
				break;

			default:
				break;
		}
	}
}