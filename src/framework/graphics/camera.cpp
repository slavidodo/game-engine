
#include "pch.h"

#include "camera.h"

#include <framework/core/scene/scene_node.h>
#include <framework/platform/window.h>
#include <glm/gtx/quaternion.hpp>

using namespace framework::graphics;

bool Camera::isViewOutdated()
{
	if (auto parent = m_parent.lock()) {
		auto transform = parent->transform();

		glm::fvec3 pTranslation = transform->translation();
		glm::fquat pRotation = transform->rotation();
		if (m_viewOutdated || pTranslation != m_parentTranslation || pRotation != m_parentRotation) {
			m_parentTranslation = pTranslation;
			m_parentRotation = pRotation;
			m_viewOutdated = true;
		}
	}

	// editor camera should update its own values
	// and will not rely on its parent
	return m_viewOutdated;
}

glm::fmat3x4 Camera::getViewMatrix()
{
	updateView();
	return m_viewMatrix;
}

glm::fmat3x4 Camera::getProjMatrix()
{
	updateProjection();
	return m_projMatrix;
}

float Camera::getAspectRatio()
{
	glm::uvec2 size = g_window.getWindowSize();
	return (float)size.x / (float)size.y;
}

void Camera::updateView()
{
	if (isViewOutdated()) {
		m_viewOutdated = false;

		glm::fmat3 rotation(glm::mat3_cast(m_parentRotation));
		glm::fmat3 rotationT = glm::transpose(rotation);

		glm::fvec3 translation = (-rotationT) * m_parentTranslation;
		glm::fmat4 viewMatrix = rotationT;

		// casts mat4x4 to mat3x4
		m_viewMatrix = glm::translate(viewMatrix, translation);
	}
}

void Camera::setProjectionType(Camera::ProjectionType type)
{
	if (type != m_projType) {
		m_projType = type;
		m_projOutdated = true;
	}
}

void Camera::setFovY(float fovY)
{
	if (m_fovY != fovY) {
		m_fovY = fovY;

		// invalidate projection matrix only if we are using perspective projection
		if (m_projType == PROJECTION_PERSPECTIVE) {
			m_projOutdated = true;
		}
	}
}

void Camera::setZNear(float zNear)
{
	if (m_zNear != zNear) {
		m_zNear = zNear;
		m_projOutdated = true;
	}
}

void Camera::setZFar(float zFar)
{
	if (m_zFar != zFar) {
		m_zFar = zFar;
		m_projOutdated = true;
	}
}

void Camera::updateProjection()
{
	if (m_projOutdated) {
		m_projOutdated = false;

		if (m_projType == PROJECTION_PERSPECTIVE) {
			m_projMatrix = getPerspectiveProjMatrix();
		}
	}
}

glm::fmat3x4 Camera::getPerspectiveProjMatrix()
{
	return glm::perspective(m_fovY, getAspectRatio(), m_zNear, m_zFar);
}