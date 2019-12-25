

#ifndef FRAMEWORK_GRAPHICS_CORE_CONTEXT
#define FRAMEWORK_GRAPHICS_CORE_CONTEXT

#include <framework/core/color.h>

namespace framework {
namespace graphics {
namespace core {

class Context {
public:

	virtual void vsync(bool value) = 0;
	virtual bool vsync() = 0;
	virtual bool vsync() const = 0;

	virtual void clearColor(framework::core::Color color) = 0;

private:

};

} // ns framework
} // ns graphics
} // ns core
#endif
