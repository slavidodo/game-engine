
#include "pch.h"

#include "Transform.h"

void Transform::SetTranslation(glm::fvec3 value)
{
	mTranslation = value;
	mMustUpdate = true;
}

void Transform::SetRotation(glm::fquat value)
{
	mRotation = value;
	mMustUpdate = true;
}

void Transform::SetRotation(glm::fvec3 eulerAngles)
{
	SetRotation(glm::fquat(eulerAngles));
}

void Transform::SetScale(glm::fvec3 value)
{
	mScale = value;
	mMustUpdate = true;
}

void Transform::Move(glm::fvec3 value)
{
	SetTranslation(GetTranslation() + value);
}

void Transform::Rotate(glm::fquat value)
{
	SetRotation(GetRotation() * value);
}

void Transform::Rotate(glm::fvec3 eulerAngles)
{
	Rotate(glm::fquat(eulerAngles));
}

void Transform::LookAt(glm::fvec3 position)
{
	// if there are pending changes, just recalculate up vector
	if (mMustUpdate)
		mUp = mRotation * glm::fvec3(0.0, 1.0, 0.0);

	glm::fmat4 rotationMat = glm::lookAt(position, mTranslation, mUp);
	glm::fquat rotationQuat = glm::quat_cast(rotationMat);
	SetRotation(glm::inverse(rotationQuat));
}

void Transform::EnsureUpdated()
{
	if (mMustUpdate) {
		mMustUpdate = false;

		mForward = mRotation * glm::fvec3(0.0, 0.0, 1.0);
		mRight = mRotation * glm::fvec3(1.0, 0.0, 0.0);
		mUp = mRotation * glm::fvec3(0.0, 1.0, 0.0);

		// update the model matrix
		mLocalToWorld = glm::translate(glm::fmat4(1.0f), mTranslation);
		mLocalToWorld = mLocalToWorld * glm::mat4_cast(mRotation);
		mLocalToWorld = glm::scale(mLocalToWorld, mScale);

		mWorldToLocal = glm::inverse(mLocalToWorld);
	}
}