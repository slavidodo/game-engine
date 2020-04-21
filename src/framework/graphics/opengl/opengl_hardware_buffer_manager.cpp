
#include <framework/pch.h>

#include "opengl_hardware_buffer_manager.h"

using namespace framework::graphics::opengl;
namespace rendering = framework::graphics::rendering;

OpenGLHardwareBufferManager::VertexBuffer* OpenGLHardwareBufferManager::createVertexBuffer(size_t verticesCount, size_t vertexSize, HardwareBuffer::Usage usage)
{
	return new OpenGLVertexBuffer(verticesCount, vertexSize, usage);
}

OpenGLHardwareBufferManager::IndexBuffer* OpenGLHardwareBufferManager::createIndexBuffer(size_t indicesCount, rendering::HardwareBuffer::Usage usage)
{
	return new OpenGLIndexBuffer(indicesCount, usage);
}