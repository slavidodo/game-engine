
#include <framework/pch.h>

#include "transform.h"

using namespace framework::core;

void Transform::translation(glm::vec3 value)
{
	m_translation = value;
	m_mustUpdate = true;
}

void Transform::rotation(glm::quat value)
{
	m_rotation = value;
	m_mustUpdate = true;
}

void Transform::scale(glm::vec3 value)
{
	m_scale = value;
	m_mustUpdate = true;
}

void Transform::updateTransform()
{
	if (!m_mustUpdate)
		return;

	// todo update transform based
}