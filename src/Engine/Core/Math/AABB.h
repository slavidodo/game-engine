
#ifndef ENGINE_CORE_MATH_AABB_H
#define ENGINE_CORE_MATH_AABB_H

class AABB
{
public:
	// TODO [honey]: use world unit
	AABB()
		: mMinimum(-0.5, -0.5, -0.5), mMaximum(0.5, 0.5, 0.5) {} // null extent

	AABB(glm::fvec3 min, glm::fvec3 max) {
		assert((min.x <= max.x && min.y <= max.y && min.z <= max.z));

		mMinimum = min;
		mMaximum = max;
	}

	AABB(float ax, float ay, float az, float bx, float by, float bz) {
		assert((ax <= bx && ay <= by && az <= bz));

		mMinimum = glm::fvec3(ax, ay, az);
		mMaximum = glm::fvec3(bx, by, bz);
	}

	AABB(const AABB& box)
		: mMinimum(box.mMinimum), mMaximum(box.mMaximum) {}

	AABB& operator=(const AABB& rhs) {
		mMinimum = rhs.mMinimum;
		mMaximum = rhs.mMaximum;

		return *this;
	}

	bool operator==(const AABB& rhs) const {
		return mMinimum == rhs.mMinimum && mMaximum == rhs.mMaximum;
	}

	bool operator!=(const AABB& rhs) const {
		return !(*this == rhs);
	}

	inline bool Intersects(glm::fvec3 v) const {
		return v.x >= mMinimum.x && v.x <= mMaximum.x
			&& v.y >= mMinimum.y && v.y <= mMaximum.y
			&& v.z >= mMinimum.z && v.z <= mMaximum.z;
	}

	inline bool Intersects(const AABB& b) const {
		return !(mMinimum.x > b.mMaximum.x
			|| mMinimum.y > b.mMaximum.y
			|| mMinimum.z > b.mMaximum.z
			|| mMaximum.x < b.mMinimum.x
			|| mMaximum.y < b.mMinimum.y
			|| mMaximum.z < b.mMinimum.z);
	}

	inline bool Contains(glm::fvec3 v) const {
		return Intersects(v);
	}

	inline bool Contains(const AABB& b) const {
		return Contains(b.mMinimum) && Contains(b.mMaximum);
	}

	inline void Scale(glm::fvec3 s) {
		mMinimum *= s;
		mMaximum *= s;
	}

	inline glm::fvec3 GetCenter() const {
		return (mMinimum + mMaximum) * 0.5f;
	}

	inline glm::fvec3 GetSize() const {
		return mMaximum - mMinimum;
	}

	inline glm::fvec3 GetMinimum() {
		return mMinimum;
	}

	inline glm::fvec3 GetMaximum() {
		return mMaximum;
	}

protected:
	glm::fvec3 mMinimum;
	glm::fvec3 mMaximum;

};

#endif // ENGINE_CORE_MATH_AABB_H
