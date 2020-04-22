
#ifndef FRAMEWORK_CORE_AABB_H
#define FRAMEWORK_CORE_AABB_H

#include <assert.h>
#include <glm/glm.hpp>


namespace framework {
	namespace core {
		namespace math {

			class AABB
			{
			public:

				// OBJECT_ACCESSOR(glm::vec3, const glm::vec3, minimum);
				// OBJECT_ACCESSOR(glm::vec3, const glm::vec3, maximum);

				// TODO [honey]: use world unit
				explicit AABB() : m_minimum(-0.5, -0.5, -0.5), m_maximum(0.5, 0.5, 0.5) {}
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

				AABB& operator=(const AABB& rhs);
				bool operator==(const AABB& rhs) const;
				bool operator!=(const AABB& rhs) const;

				inline bool intersects(const glm::vec3& v) const;
				inline bool intersects(const AABB& b) const;

				inline bool contains(const glm::vec3& v) const;
				inline bool contains(const AABB& b) const;

				inline void scale(const glm::vec3& s);

				glm::vec3 getCenter(void) const;
				glm::vec3 getSize(void) const;

			protected:
				glm::vec3 m_minimum;
				glm::vec3 m_maximum;

			};

		} // ns math
	} // ns core
} // ns framework

#endif