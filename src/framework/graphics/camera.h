
#ifndef FRAMEWORK_GRAPHICS_CAMERA_H
#define FRAMEWORK_GRAPHICS_CAMERA_H

#include <framework/core/scene/object.h>

namespace framework {
namespace graphics {

class Camera;
typedef std::shared_ptr<Camera> Camera_ptr;

class Camera : public framework::core::scene::Object
{
public:
	enum ProjectionType {
		PROJECTION_PERSPECTIVE,
	};

	virtual bool isViewOutdated();
	virtual glm::fmat3x4 getViewMatrix();
	virtual glm::fmat3x4 getProjMatrix();

	virtual float getAspectRatio();

	ProjectionType getProjectionType() { return m_projType; }
	float getFovY() { return m_fovY; }
	float getZNear() { return m_zNear; }
	float getZFar() { return m_zFar; }

	void setProjectionType(Camera::ProjectionType type);
	void setFovY(float value);
	void setZNear(float zNear);
	void setZFar(float zFar);

protected:
	virtual void updateView();
	virtual void updateProjection();

	virtual glm::fmat3x4 getPerspectiveProjMatrix();

	glm::fmat3x4 m_viewMatrix;
	glm::fmat3x4 m_projMatrix;

	glm::fvec3 m_parentTranslation;
	glm::fquat m_parentRotation;

	ProjectionType m_projType = ProjectionType::PROJECTION_PERSPECTIVE;

	float m_fovY = 40;
	float m_zNear = 0.3f;
	float m_zFar = 1000.f;

	bool m_viewOutdated = true;
	bool m_projOutdated = true;
};

} // ns graphics
} // ns framework

#endif