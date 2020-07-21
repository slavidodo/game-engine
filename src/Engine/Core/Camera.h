
#ifndef ENGINE_CORE_CAMERA_H
#define ENGINE_CORE_CAMERA_H

#include "Object.h"
#include "../Scene/Actor.h"
#include "../Scene/SceneNode.h"

class Camera;
typedef std::shared_ptr<Camera> Camera_ptr;

class Camera : public Object
{
public:
	enum ProjectionType {
		PROJECTION_PERSPECTIVE
	};

	static Camera_ptr CreatePerspectiveCamera(float InFovY, float InZNear, float InZFar) {
		Camera_ptr Cam = std::make_shared<Camera>();
		Cam->SetProjectionType(Camera::ProjectionType::PROJECTION_PERSPECTIVE);
		Cam->SetFovY(InFovY);
		Cam->SetZNear(InZNear);
		Cam->SetZFar(InZFar);

		Cam->mPitch = 0.0f;
		Cam->mYaw = -90.0f;
		Cam->mPosition = glm::fvec3(0.0f);
		Cam->mFront = glm::fvec3(0.0f, 0.0f, -1.0f);
		Cam->SetLocalTranslation(glm::fvec3(0.0f, 0.0f, 0.0f));
		
		return Cam;
	}

	virtual bool IsViewOutdated();
	virtual glm::fmat4x4 GetViewMatrix();
	virtual glm::fmat4x4 GetProjMatrix();

	virtual float GetAspectRatio();

	ProjectionType GetProjectionType() { return mProjType; }
	float GetFovY() { return mFovY; }
	float GetZNear() { return mZNear; }
	float GetZFar() { return mZFar; }

	void SetProjectionType(ProjectionType type);
	void SetFovY(float value);
	void SetZNear(float zNear);
	void SetZFar(float zFar);

	void SetEditorTranslation(glm::fvec3 Value) { mEditorTranslation = Value; }
	void SetEditorRotation(glm::fquat Value) { mEditorRotation = Value; }
	void SetEditorRotation(glm::fvec3 EulerAngles) { mEditorRotation = glm::fquat(EulerAngles); }
	void SetEditorRotationDegrees(glm::fvec3 EulerAngles) { SetEditorRotation(glm::radians(EulerAngles)); }


	void SetParent(Actor_ptr parent);
	void SetLocalTranslation(glm::fvec3 translation);
	void Rotate(float xDelta, float yDelta);

	float mPitch, mYaw;
	glm::fvec3 mPosition, mFront;

protected:
	virtual void UpdateView();
	virtual void UpdateProjection();

	virtual glm::fmat4x4 GetPerspectiveProjMatrix();

	glm::fmat4x4 mViewMatrix;
	glm::fmat4x4 mProjMatrix;

	glm::fvec3 mEditorTranslation;
	glm::fquat mEditorRotation;

	glm::fvec3 mParentTranslation;
	glm::fquat mParentRotation;

	ProjectionType mProjType = ProjectionType::PROJECTION_PERSPECTIVE;

	float mFovY = 40;
	float mZNear = 0.3f;
	float mZFar = 1000.f;

	bool mViewOutdated = true;
	bool mProjOutdated = true;


	std::weak_ptr<Actor> mTempParent;
	glm::fvec3 mLocalTranslation;
};

#endif // ENGINE_CORE_CAMERA_H