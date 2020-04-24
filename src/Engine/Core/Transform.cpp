
#include "pch.h"

#include "Transform.h"

void Transform::translation(glm::fvec3 value)
{
	m_translation = value;
	m_mustUpdate = true;
}

void Transform::rotation(glm::fquat value)
{
	m_rotation = value;
	m_mustUpdate = true;
}

void Transform::rotation(glm::fvec3 eulerAngles)
{
	rotation(glm::fquat(eulerAngles));
}

void Transform::scale(glm::fvec3 value)
{
	m_scale = value;
	m_mustUpdate = true;
}

void Transform::move(glm::fvec3 value)
{
	translation(translation() + value);
}

void Transform::rotate(glm::fquat value)
{
	rotation(rotation() * value);
}

void Transform::rotate(glm::fvec3 eulerAngles)
{
	rotate(glm::fquat(eulerAngles));
}

void Transform::lookAt(glm::fvec3 position)
{
	// if there are pending changes, just recalculate up vector
	if (m_mustUpdate)
		m_up = m_rotation * glm::fvec3(0.0, 1.0, 0.0);

	glm::fmat4 rotationMat = glm::lookAt(position, m_translation, m_up);
	glm::fquat rotationQuat = glm::quat_cast(rotationMat);
	rotation(glm::inverse(rotationQuat));
}

void Transform::ensureUpdated()
{
	if (m_mustUpdate) {
		m_mustUpdate = false;

		m_forward = m_rotation * glm::fvec3(0.0, 0.0, 1.0);
		m_right = m_rotation * glm::fvec3(1.0, 0.0, 0.0);
		m_up = m_rotation * glm::fvec3(0.0, 1.0, 0.0);

		// update the model matrix
		m_localToWorld = glm::fmat4(1.0f);
		m_localToWorld = glm::translate(m_localToWorld, m_translation);
		m_localToWorld = m_localToWorld * glm::mat4_cast(m_rotation);
		m_localToWorld = glm::scale(m_localToWorld, m_scale);

		m_worldToLocal = glm::inverse(m_localToWorld);
	}
}