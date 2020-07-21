
#ifndef ENGINE_CORE_TRANSFORM_H
#define ENGINE_CORE_TRANSFORM_H

class Transform;
typedef std::shared_ptr<Transform> Transform_ptr;

class Transform
{
public:
	Transform()
		: Transform(glm::fvec3(0.0f), glm::fquat(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) {}

	// euler angles in radians
	Transform(glm::fvec3 translation, glm::fvec3 eulerAngles, glm::fvec3 scale = glm::fvec3(1.0f))
		: Transform(translation, glm::fquat(eulerAngles), scale) {}

	Transform(glm::fvec3 translation, glm::fquat rotation, glm::fvec3 scale = glm::fvec3(1.0f))
		: mTranslation(translation), mRotation(rotation), mScale(scale), 
		  mForward(0.0f, 0.0f, 1.0f), mRight(1.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f),
		  mLocalToWorld(0.0f), mWorldToLocal(0.0f)
	{
	}

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

	// local/world matrices should be Updated internally
	glm::fvec3 GetTranslation() const { return mTranslation; }
	glm::fquat GetRotation() const { return mRotation; }
	glm::fvec3 GetScale() const { return mScale; }

	glm::fmat4 GetLocalToWorld() const { return mLocalToWorld; }
	glm::fmat4 GetWorldToLocal() const { return mWorldToLocal; }

	glm::fvec3 GetForward() const { return mForward; }
	glm::fvec3 GetRight() const { return mRight; }
	glm::fvec3 GetUp() const { return mUp; }

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