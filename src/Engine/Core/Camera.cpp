
#include "pch.h"

#include "Camera.h"

#include "../Scene/SceneNode.h"
#include "../Platform/Window.h"

bool Camera::IsViewOutdated()
{
	glm::fvec3 DesiredParentTranslation;
	glm::fquat DesiredParentRotation;
	
	if (Actor_ptr parent = mTempParent.lock()) {
		Transform_ptr transform = parent->GetTransform();
		DesiredParentTranslation = transform->GetTranslation();
		//DesiredParentRotation = transform->GetRotation();
	}
	else {
		DesiredParentTranslation = mEditorTranslation;
		DesiredParentRotation = mEditorRotation;
	}

	if (mViewOutdated || DesiredParentTranslation != mParentTranslation || DesiredParentRotation != mParentRotation) {
		mParentTranslation = DesiredParentTranslation;
		//mParentRotation = DesiredParentRotation;
		mViewOutdated = true;
	}

	return mViewOutdated;
}

glm::fmat4x4 Camera::GetViewMatrix()
{
	UpdateView();
	return mViewMatrix;
}

glm::fmat4x4 Camera::GetProjMatrix()
{
	UpdateProjection();
	return mProjMatrix;
}

float Camera::GetAspectRatio()
{
	glm::uvec2 size = gWindow.GetWindowSize();
	return (float)size.x / (float)size.y;
}

void Camera::UpdateView()
{
	if (IsViewOutdated()) {
		mViewOutdated = false;

		//std::cout << mParentRotation.x << " " << mParentRotation.y << " " << mParentRotation.z << " " << mParentRotation.w << std::endl;

		glm::fmat3 rotation(glm::mat3_cast(mParentRotation));
		glm::fmat3 rotationT = glm::transpose(rotation);

		glm::fvec3 translation = (-rotationT) * (mParentTranslation + mLocalTranslation);
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

glm::fmat4x4 Camera::GetPerspectiveProjMatrix()
{
	std::cout << "Updating Proj: " << GetFovY() << ", " << glm::radians(GetFovY()) << ", " << GetAspectRatio()
		<< ", " << mZNear << ", " << mZFar << std::endl;
	return glm::perspective(glm::radians(GetFovY()), GetAspectRatio(), mZNear, mZFar);
}