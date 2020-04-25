
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
		: mTranslation(translation), mRotation(rotation), mScale(scale), mForward(0.0f), mRight(0.0f), mUp(0.0f),
			mLocalToWorld(0.0f), mWorldToLocal(0.0f)
	{
	}

	OBJECT_GETACCESSOR(glm::fvec3, glm::fvec3, Translation);
	OBJECT_GETACCESSOR(glm::fquat, glm::fquat, Rotation);
	OBJECT_GETACCESSOR(glm::fvec3, glm::fvec3, Scale);

	void SetTranslation(glm::fvec3 value);
	void SetRotation(glm::fvec3 eulerAngles);
	void SetRotation(glm::fquat value);
	void SetScale(glm::fvec3 value);

	void Move(glm::fvec3 value);
	void Rotate(glm::fquat value);
	void Rotate(glm::fvec3 eulerAngles);

	void LookAt(glm::fvec3 position);
	void LookAt(Transform& other) {
		LookAt(other.GetTranslation());
	}

	// local/world matrices should be updated internally
	OBJECT_GETACCESSOR(glm::fmat4, glm::fmat4, LocalToWorld);
	OBJECT_GETACCESSOR(glm::fmat4, glm::fmat4, WorldToLocal);

	OBJECT_GETACCESSOR(glm::fvec3, glm::fvec3, Forward);
	OBJECT_GETACCESSOR(glm::fvec3, glm::fvec3, Right);
	OBJECT_GETACCESSOR(glm::fvec3, glm::fvec3, Up);

	void EnsureUpdated();

private:

	glm::fvec3 mTranslation;
	glm::fquat mRotation;
	glm::fvec3 mScale;
	glm::fmat4 mLocalToWorld;
	glm::fmat4 mWorldToLocal;

	// misc
	glm::fvec3 mForward;
	glm::fvec3 mRight;
	glm::fvec3 mUp;

	bool mMustUpdate = true;
};

#endif // ENGINE_CORE_TRANSFORM_H