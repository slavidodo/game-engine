
#include "pch.h"

#include "Material.h"

void Material::ShaderPass() const
{
	auto it = m_typedValues.begin();
	auto end = m_typedValues.end();
	for (; it != end; it++) {
		const std::string& uname = (*it).first;
		const void* value = (*it).second.value;
		size_t size = (*it).second.size;

		switch ((*it).second.type) {
			case MTV_TYPE_BOOL1:
				m_shader->SetBool(uname, glm::bvec1(((bool*)value)[0]));
				break;
			case MTV_TYPE_BOOL2:
				m_shader->SetBool2(uname, glm::make_vec2((bool*)value));
				break;
			case MTV_TYPE_BOOL3:
				m_shader->SetBool3(uname, glm::make_vec3((bool*)value));
				break;
			case MTV_TYPE_BOOL4:
				m_shader->SetBool4(uname, glm::make_vec4((bool*)value));
				break;
			case MTV_TYPE_FLOAT1:
				m_shader->SetFloat(uname, glm::fvec1(((float*)value)[0]));
				break;
			case MTV_TYPE_FLOAT2:
				m_shader->SetFloat2(uname, glm::make_vec2((float*)value));
				break;
			case MTV_TYPE_FLOAT3:
				m_shader->SetFloat3(uname, glm::make_vec3((float*)value));
				break;
			case MTV_TYPE_FLOAT4:
				m_shader->SetFloat4(uname, glm::make_vec4((float*)value));
				break;
			case MTV_TYPE_INT1:
				m_shader->SetInt(uname, glm::ivec1(((int*)value)[0]));
				break;
			case MTV_TYPE_INT2:
				m_shader->SetInt2(uname, glm::make_vec2((int*)value));
				break;
			case MTV_TYPE_INT3:
				m_shader->SetInt3(uname, glm::make_vec3((int*)value));
				break;
			case MTV_TYPE_INT4:
				m_shader->SetInt4(uname, glm::make_vec4((int*)value));
				break;
			case MTV_TYPE_MAT3x3:
				m_shader->SetMat3x3(uname, glm::make_mat3x3((float*)value));
				break;
			case MTV_TYPE_MAT4x4:
				m_shader->SetMat4x4(uname, glm::make_mat4x4((float*)value));
				break;
			case MTV_TYPE_BOOL_ARRAY:
				// TODO;
				break;
			case MTV_TYPE_FLOAT_ARRAY:
				m_shader->SetFloatArray(uname, (glm::fvec1*)value, size);
				break;
			case MTV_TYPE_FLOAT4_ARRAY:
				m_shader->SetFloat4Array(uname, (glm::fvec4*)value, size);
				break;

			default:
				break;
		}
	}
}