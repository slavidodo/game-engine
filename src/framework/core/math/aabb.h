
#ifndef FRAMEWORK_CORE_MATH_AABB_H
#define FRAMEWORK_CORE_MATH_AABB_H

namespace framework {
namespace core {
namespace math {

class AABB
{
public:
	// TODO [honey]: use world unit
	AABB()
		: m_minimum(-0.5, -0.5, -0.5), m_maximum(0.5, 0.5, 0.5) {} // null extent

	AABB(glm::fvec3 min, glm::fvec3 max) {
		assert((min.x <= max.x && min.y <= max.y && min.z <= max.z));

		m_minimum = min;
		m_maximum = max;
	}

	AABB(float ax, float ay, float az, float bx, float by, float bz) {
		assert((ax <= bx && ay <= by && az <= bz));

		m_minimum = glm::fvec3(ax, ay, az);
		m_maximum = glm::fvec3(bx, by, bz);
	}

	AABB(const AABB& box)
		: m_minimum(box.m_minimum), m_maximum(box.m_maximum) {}

	AABB& operator=(const AABB& rhs) {
		m_minimum = rhs.m_minimum;
		m_maximum = rhs.m_maximum;

		return *this;
	}

	bool operator==(const AABB& rhs) const {
		return m_minimum == rhs.m_minimum && m_maximum == rhs.m_maximum;
	}

	bool operator!=(const AABB& rhs) const {
		return !(*this == rhs);
	}

	inline bool intersects(glm::fvec3 v) const {
		return v.x >= m_minimum.x && v.x <= m_maximum.x
			&& v.y >= m_minimum.y && v.y <= m_maximum.y
			&& v.z >= m_minimum.z && v.z <= m_maximum.z;
	}

	inline bool intersects(const AABB& b) const {
		return !(m_minimum.x > b.m_maximum.x
			|| m_minimum.y > b.m_maximum.y
			|| m_minimum.z > b.m_maximum.z
			|| m_maximum.x < b.m_minimum.x
			|| m_maximum.y < b.m_minimum.y
			|| m_maximum.z < b.m_minimum.z);
	}

	inline bool contains(glm::fvec3 v) const {
		return intersects(v);
	}

	inline bool contains(const AABB& b) const {
		return contains(b.m_minimum) && contains(b.m_maximum);
	}

	inline void scale(glm::fvec3 s) {
		m_minimum *= s;
		m_maximum *= s;
	}

	inline glm::fvec3 getCenter() const {
		return (m_minimum + m_maximum) * 0.5f;
	}

	inline glm::fvec3 getSize() const {
		return m_maximum - m_minimum;
	}

	inline glm::fvec3 getMinimum() {
		return m_minimum;
	}

	inline glm::fvec3 getMaximum() {
		return m_maximum;
	}

protected:
	glm::fvec3 m_minimum;
	glm::fvec3 m_maximum;

};

} // ns math
} // ns core
} // ns framework

#endif // FRAMEWORK_CORE_MATH_AABB_H
