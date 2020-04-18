
#ifndef FRAMEWORK_CORE_UTILS_H
#define FRAMEWORK_CORE_UTILS_H

namespace framework {
namespace core {

class Utils
{
public:
	static glm::fvec4 ColorBlack;
	static glm::fvec4 ColorWhite;
	static glm::fvec4 ColorRed;
	static glm::fvec4 ColorGreen;
	static glm::fvec4 ColorBlue;
	static glm::fvec4 ColorYellow;
	static glm::fvec4 ColorOrange;
	static glm::fvec4 ColorCyan;

private:
	Utils() {}
};

} // ns core
} // ns framework

#endif