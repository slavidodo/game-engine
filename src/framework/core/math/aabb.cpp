
#include <framework/pch.h>

#include "aabb.h"

using namespace framework::core::math;

AABB& AABB::operator=(const AABB& rhs) {
	m_minimum = rhs.m_minimum;
	m_maximum = rhs.m_maximum;

	return *this;
}

bool AABB::operator==(const AABB& rhs) const {
	return m_minimum == rhs.m_minimum && m_maximum == rhs.m_maximum;
}

bool AABB::operator!=(const AABB& rhs) const {
	return !(*this == rhs);
}

inline bool AABB::intersects(const glm::vec3& v) const {
	return v.x >= m_minimum.x && v.x <= m_maximum.x
		&& v.y >= m_minimum.y && v.y <= m_maximum.y
		&& v.z >= m_minimum.z && v.z <= m_maximum.z;
}

inline bool AABB::intersects(const AABB& b) const {
	return !(m_minimum.x > b.m_maximum.x
		  || m_minimum.y > b.m_maximum.y
		  || m_minimum.z > b.m_maximum.z
		  || m_maximum.x < b.m_minimum.x
		  || m_maximum.y < b.m_minimum.y
		  || m_maximum.z < b.m_minimum.z);
}

inline bool AABB::contains(const glm::vec3& v) const {
	return intersects(v);
}

inline bool AABB::contains(const AABB& b) const {
	return contains(b.m_minimum) && contains(b.m_maximum);
}

inline void AABB::scale(const glm::vec3& s) {
	m_minimum *= s;
	m_maximum *= s;
}

glm::vec3 AABB::getCenter(void) const {
	return (m_minimum + m_maximum) * 0.5f;
}

glm::vec3 AABB::getSize(void) const {
	return m_maximum - m_minimum;
}
