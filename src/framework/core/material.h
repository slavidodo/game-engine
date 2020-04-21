
#ifndef FRAMEWORK_CORE_MATERIAL_H
#define FRAMEWORK_CORE_MATERIAL_H

#include <framework/graphics/core/shader.h>

namespace framework {
namespace core {

class Material;
typedef std::shared_ptr<Material> Material_ptr;
typedef std::weak_ptr<Material> Material_weakPtr;

class Material
{
	typedef graphics::core::Shader_ptr Shader_ptr;

public:
	explicit Material(Shader_ptr shader)
		: m_shader(shader) {
		assert(shader == nullptr);
	}

	void setBool(std::string global, bool value)			{ m_shader->setBool(std::move(global), glm::bvec1(value)); }
	void setBool(std::string global, glm::bvec1 value)		{ m_shader->setBool(std::move(global), value); }
	void setBool2(std::string global, glm::bvec2 value)		{ m_shader->setBool2(std::move(global), value); }
	void setBool3(std::string global, glm::bvec3 value)		{ m_shader->setBool3(std::move(global), value); }
	void setBool4(std::string global, glm::bvec4 value)		{ m_shader->setBool4(std::move(global), value); }

	void setInt(std::string global, int32_t value)			{ m_shader->setInt(std::move(global), glm::ivec1(value)); }
	void setInt(std::string global, glm::ivec1 value)		{ m_shader->setInt(std::move(global), value); }
	void setInt2(std::string global, glm::ivec2 value)		{ m_shader->setInt2(std::move(global), value); }
	void setInt3(std::string global, glm::ivec3 value)		{ m_shader->setInt3(std::move(global), value); }
	void setInt4(std::string global, glm::ivec4 value)		{ m_shader->setInt4(std::move(global), value); }

	void setFloat(std::string global, float value)			{ m_shader->setFloat(std::move(global), glm::fvec1(value)); }
	void setFloat(std::string global, glm::fvec1 value)		{ m_shader->setFloat(std::move(global), value); }
	void setFloat2(std::string global, glm::fvec2 value)	{ m_shader->setFloat2(std::move(global), value); }
	void setFloat3(std::string global, glm::fvec3 value)	{ m_shader->setFloat3(std::move(global), value); }
	void setFloat4(std::string global, glm::fvec4 value)	{ m_shader->setFloat4(std::move(global), value); }

	// commonly used attributes
	void setFloatArray(std::string global, std::vector<glm::fvec1> value) {
		setFloatArray(std::move(global), value.data(), value.size());
	}
	void setFloatArray(std::string global, glm::fvec1* value, std::size_t size) {
		m_shader->setFloatArray(std::move(global), value, size);
	}
	void setFloat2Array(std::string global, std::vector<glm::fvec2> value) {
		m_shader->setFloat2Array(std::move(global), value.data(), value.size());
	}
	void setFloat2Array(std::string global, glm::fvec2* value, std::size_t size) {
		m_shader->setFloat2Array(std::move(global), value, size);
	}
	void setFloat3Array(std::string global, std::vector<glm::fvec3> value) {
		m_shader->setFloat3Array(std::move(global), value.data(), value.size());
	}
	void setFloat3Array(std::string global, glm::fvec3* value, std::size_t size) {
		m_shader->setFloat3Array(global, value, size);
	}
	void setFloat4Array(std::string global, std::vector<glm::fvec4> value) {
		m_shader->setFloat4Array(std::move(global), value.data(), value.size());
	}
	void setFloat4Array(std::string global, glm::fvec4* value, std::size_t size) {
		m_shader->setFloat4Array(std::move(global), value, size);
	}

protected:
	Shader_ptr m_shader;
};

} // ns core
} // ns framework

#endif