
#ifndef FRAMEWORK_MANAGERS_HARDWARE_BUFFER_MANAGER_H
#define FRAMEWORK_MANAGERS_HARDWARE_BUFFER_MANAGER_H

#include <framework/graphics/rendering/vertex_buffer.h>
#include <framework/graphics/rendering/index_buffer.h>

namespace framework {
namespace managers {

class HardwareBufferManager
{
protected:
	using HardwareBuffer = graphics::rendering::HardwareBuffer;
	using VertexBuffer = graphics::rendering::VertexBuffer;
	using IndexBuffer = graphics::rendering::IndexBuffer;

public:
	static HardwareBufferManager& instance() {
		return *s_instance;
	}

	static void instance(HardwareBufferManager* instance) {
		s_instance = instance;
	}

	virtual VertexBuffer* createVertexBuffer(size_t verticesCount, size_t vertexSize, HardwareBuffer::Usage usage) = 0;
	virtual IndexBuffer* createIndexBuffer(size_t indicesCount, HardwareBuffer::Usage usage) = 0;

private:
	static HardwareBufferManager* s_instance;

};

} // ns managers
} // ns framework

#endif // FRAMEWORK_MANAGERS_HARDWARE_BUFFER_MANAGER_H