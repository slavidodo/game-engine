
#include "pch.h"

#include "Camera.h"

#include "../Scene/SceneNode.h"
#include "../Platform/Window.h"

bool Camera::IsViewOutdated()
{
	if (auto parent = mParent.lock()) {
		auto transform = parent->GetTransform();

		glm::fvec3 pTranslation = transform->GetTranslation();
		glm::fquat pRotation = transform->GetRotation();
		if (mViewOutdated || pTranslation != mParentTranslation || pRotation != mParentRotation) {
			mParentTranslation = pTranslation;
			mParentRotation = pRotation;
			mViewOutdated = true;
		}
	}

	// editor camera should update its own values
	// and will not rely on its parent
	return mViewOutdated;
}

glm::fmat3x4 Camera::GetViewMatrix()
{
	UpdateView();
	return mViewMatrix;
}

glm::fmat3x4 Camera::GetProjMatrix()
{
	UpdateProjection();
	return mProjMatrix;
}

float Camera::GetAspectRatio()
{
	glm::uvec2 size = g_window.getWindowSize();
	return (float)size.x / (float)size.y;
}

void Camera::UpdateView()
{
	if (IsViewOutdated()) {
		mViewOutdated = false;

		glm::fmat3 rotation(glm::mat3_cast(mParentRotation));
		glm::fmat3 rotationT = glm::transpose(rotation);

		glm::fvec3 translation = (-rotationT) * mParentTranslation;
		glm::fmat4 viewMatrix = rotationT;

		// casts mat4x4 to mat3x4
		mViewMatrix = glm::translate(viewMatrix, translation);
	}
}

void Camera::SetProjectionType(ProjectionType type)
{
	if (type != mProjType) {
		mProjType = type;
		mProjOutdated = true;
	}
}

void Camera::SetFovY(float fovY)
{
	if (mFovY != fovY) {
		mFovY = fovY;

		// invalidate projection matrix only if we are using perspective projection
		if (mProjType == PROJECTION_PERSPECTIVE) {
			mProjOutdated = true;
		}
	}
}

void Camera::SetZNear(float zNear)
{
	if (mZNear != zNear) {
		mZNear = zNear;
		mProjOutdated = true;
	}
}

void Camera::SetZFar(float zFar)
{
	if (mZFar != zFar) {
		mZFar = zFar;
		mProjOutdated = true;
	}
}

void Camera::UpdateProjection()
{
	if (mProjOutdated) {
		mProjOutdated = false;

		if (mProjType == PROJECTION_PERSPECTIVE) {
			mProjMatrix = GetPerspectiveProjMatrix();
		}
	}
}

glm::fmat3x4 Camera::GetPerspectiveProjMatrix()
{
	return glm::perspective(GetFovY(), GetAspectRatio(), mZNear, mZFar);
}