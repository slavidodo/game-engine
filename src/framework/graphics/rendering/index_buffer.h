
#ifndef FRAMEWORK_GRAPHICS_RENDERING_INDEX_BUFFER_H
#define FRAMEWORK_GRAPHICS_RENDERING_INDEX_BUFFER_H

#include "hardware_buffer.h"

namespace framework {
namespace graphics {
namespace rendering {

class IndexBuffer : public HardwareBuffer
{
public:
	IndexBuffer(size_t indicesCount, Usage usage)
		: HardwareBuffer(usage), m_indicesCount(indicesCount) {
		m_size = indicesCount * sizeof(uint32_t);
	}

	constexpr size_t getIndexSize() {
		return sizeof(size_t);
	}

private:
	size_t m_indicesCount;
};

} // ns rendering
} // ns graphics
} // ns framework

#endif // FRAMEWORK_GRAPHICS_RENDERING_INDEX_BUFFER_H
