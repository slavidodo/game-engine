
#ifndef FRAMEWORK_CORE_COLOR_H
#define FRAMEWORK_CORE_COLOR_H

namespace framework {
namespace core {

class Color
{
public:
	static Color black;
	static Color white;
	static Color red;
	static Color green;
	static Color blue;
	static Color yellow;
	static Color orange;
	static Color cyan;

	Color(float r, float g, float b) : m_r(r), m_g(g), m_b(b), m_a(1) {}
	Color(float r, float g, float b, float a) : m_r(r), m_g(g), m_b(b), m_a(a) {}

	OBJECT_ACCESSOR(float, r)
	OBJECT_ACCESSOR(float, g)
	OBJECT_ACCESSOR(float, b)
	OBJECT_ACCESSOR(float, a)

private:
	float m_r = 0;
	float m_b = 0;
	float m_g = 0;
	float m_a = 0;
};

} // ns framework
} // ns core

#endif