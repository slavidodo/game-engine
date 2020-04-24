
#ifndef ENGINE_CORE_TRANSFORM_H
#define ENGINE_CORE_TRANSFORM_H

class Transform;
typedef std::shared_ptr<Transform> Transform_ptr;

class Transform
{
public:
	// euler angles in radians
	Transform(glm::fvec3 translation, glm::fvec3 eulerAngles, glm::fvec3 scale)
		: Transform(translation, glm::fquat(eulerAngles), scale) {}

	Transform(glm::fvec3 translation, glm::fquat rotation, glm::fvec3 scale)
		: m_translation(translation), m_rotation(rotation), m_scale(scale), m_forward(0.0f), m_right(0.0f), m_up(0.0f),
			m_localToWorld(0.0f), m_worldToLocal(0.0f)
	{
	}

	OBJECT_GETACCESSOR(glm::fvec3, glm::fvec3, translation);
	OBJECT_GETACCESSOR(glm::fquat, glm::fquat, rotation);
	OBJECT_GETACCESSOR(glm::fvec3, glm::fvec3, scale);

	void translation(glm::fvec3 value);
	void rotation(glm::fvec3 eulerAngles);
	void rotation(glm::fquat value);
	void scale(glm::fvec3 value);

	void move(glm::fvec3 value);
	void rotate(glm::fquat value);
	void rotate(glm::fvec3 eulerAngles);

	void lookAt(glm::fvec3 position);
	void lookAt(Transform& other) {
		lookAt(other.translation());
	}

	// local/world matrices should be updated internally
	OBJECT_GETACCESSOR(glm::fmat4, glm::fmat4, localToWorld);
	OBJECT_GETACCESSOR(glm::fmat4, glm::fmat4, worldToLocal);

	OBJECT_GETACCESSOR(glm::fvec3, glm::fvec3, forward);
	OBJECT_GETACCESSOR(glm::fvec3, glm::fvec3, right);
	OBJECT_GETACCESSOR(glm::fvec3, glm::fvec3, up);

	void ensureUpdated();

private:

	glm::fvec3 m_translation;
	glm::fquat m_rotation;
	glm::fvec3 m_scale;
	glm::fmat4 m_localToWorld;
	glm::fmat4 m_worldToLocal;

	// misc
	glm::fvec3 m_forward;
	glm::fvec3 m_right;
	glm::fvec3 m_up;

	bool m_mustUpdate = true;
};

#endif // ENGINE_CORE_TRANSFORM_H