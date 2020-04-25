
#ifndef ENGINE_CORE_CAMERA_H
#define ENGINE_CORE_CAMERA_H

#include "Object.h"

class Camera;
typedef std::shared_ptr<Camera> Camera_ptr;

class Camera : public Object
{
public:
	enum ProjectionType {
		PROJECTION_PERSPECTIVE,
	};

	virtual bool IsViewOutdated();
	virtual glm::fmat3x4 GetViewMatrix();
	virtual glm::fmat3x4 GetProjMatrix();

	virtual float GetAspectRatio();

	OBJECT_GETACCESSOR(ProjectionType, ProjectionType, ProjType);
	OBJECT_GETACCESSOR(float, float, FovY);
	OBJECT_GETACCESSOR(float, float, ZNear);
	OBJECT_GETACCESSOR(float, float, ZFar);

	void SetProjectionType(ProjectionType type);
	void SetFovY(float value);
	void SetZNear(float zNear);
	void SetZFar(float zFar);

protected:
	virtual void UpdateView();
	virtual void UpdateProjection();

	virtual glm::fmat3x4 GetPerspectiveProjMatrix();

	glm::fmat3x4 mViewMatrix;
	glm::fmat3x4 mProjMatrix;

	glm::fvec3 mParentTranslation;
	glm::fquat mParentRotation;

	ProjectionType mProjType = ProjectionType::PROJECTION_PERSPECTIVE;

	float mFovY = 40;
	float mZNear = 0.3f;
	float mZFar = 1000.f;

	bool mViewOutdated = true;
	bool mProjOutdated = true;
};

#endif // ENGINE_CORE_CAMERA_H