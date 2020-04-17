
#ifndef FRAMEWORK_CORE_TRANSFORM_H
#define FRAMEWORK_CORE_TRANSFORM_H

namespace framework {
namespace core {

class Transform
{
public:
	explicit Transform(glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
		: m_translation(translation), m_rotation(rotation), m_scale(scale) {}

	OBJECT_ACCESSOR(glm::vec3, glm::vec3, translation);
	OBJECT_ACCESSOR(glm::quat, glm::quat, rotation);
	OBJECT_ACCESSOR(glm::vec3, glm::vec3, scale);
	OBJECT_ACCESSOR(glm::mat4, glm::mat4, localToWorld);
	OBJECT_ACCESSOR(glm::mat4, glm::mat4, worldToLocal);

private:
	glm::vec3 m_translation;
	glm::quat m_rotation;
	glm::vec3 m_scale;
	glm::mat4 m_localToWorld;
	glm::mat4 m_worldToLocal;
};

} // ns core
} // ns framework

#endif