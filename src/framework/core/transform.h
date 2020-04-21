
#ifndef FRAMEWORK_CORE_TRANSFORM_H
#define FRAMEWORK_CORE_TRANSFORM_H

namespace framework {
namespace core {

class Transform
{
public:
	explicit Transform(glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
		: m_translation(translation), m_rotation(rotation), m_scale(scale) {}

	OBJECT_GETACCESSOR(glm::vec3, glm::vec3, translation);
	OBJECT_GETACCESSOR(glm::quat, glm::quat, rotation);
	OBJECT_GETACCESSOR(glm::vec3, glm::vec3, scale);

	void translation(glm::vec3 value);
	void rotation(glm::quat value);
	void scale(glm::vec3 value);

	// local/world matrices should be updated internally
	OBJECT_GETACCESSOR(glm::mat4, glm::mat4, localToWorld);
	OBJECT_GETACCESSOR(glm::mat4, glm::mat4, worldToLocal);

private:
	void updateTransform();

	glm::vec3 m_translation;
	glm::quat m_rotation;
	glm::vec3 m_scale;
	glm::mat4 m_localToWorld;
	glm::mat4 m_worldToLocal;

	bool m_mustUpdate = true;
};

} // ns core
} // ns framework

#endif