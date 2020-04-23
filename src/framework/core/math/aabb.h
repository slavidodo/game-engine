
#ifndef FRAMEWORK_CORE_MATH_AABB_H
#define FRAMEWORK_CORE_MATH_AABB_H

namespace framework {
namespace core {
namespace math {

class AABB
{
public:

	OBJECT_ACCESSOR(glm::vec3, glm::vec3, minimum);
	OBJECT_ACCESSOR(glm::vec3, glm::vec3, maximum);

	// TODO [honey]: use world unit
	AABB() : m_minimum(-0.5, -0.5, -0.5), m_maximum(0.5, 0.5, 0.5) {}
	AABB(const glm::vec3& min, const glm::vec3& max) {
		assert((min.x <= max.x && min.y <= max.y && min.z <= max.z));

		m_minimum = min;
		m_maximum = max;
	}
	AABB(float ax, float ay, float az, float bx, float by, float bz) {
		assert((ax <= bx && ay <= by && az <= bz));

		m_minimum = glm::vec3(ax, ay, az);
		m_maximum = glm::vec3(bx, by, bz);
	}
	AABB(const AABB& box) : m_minimum(box.m_minimum), m_maximum(box.m_maximum) {}

	~AABB() = default;

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

	inline bool intersects(const glm::vec3& v) const {
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

	inline bool contains(const glm::vec3& v) const {
		return intersects(v);
	}

	inline bool contains(const AABB& b) const {
		return contains(b.m_minimum) && contains(b.m_maximum);
	}

	inline void scale(const glm::vec3& s) {
		m_minimum *= s;
		m_maximum *= s;
	}

	inline glm::vec3 getCenter(void) const {
		return (m_minimum + m_maximum) * 0.5f;
	}

	inline glm::vec3 getSize(void) const {
		return m_maximum - m_minimum;
	}

protected:
	glm::vec3 m_minimum;
	glm::vec3 m_maximum;

};

} // ns math
} // ns core
} // ns framework

#endif // FRAMEWORK_CORE_MATH_AABB_H
