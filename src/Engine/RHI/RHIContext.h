
#ifndef ENGINE_RHI_RHICONTEXT_H
#define ENGINE_RHI_RHICONTEXT_H

#include "../Core/Camera.h"
#include "../Core/Material.h"
#include "../Core/Mesh.h"
#include "RHIBuffer.h"

/*
 * represepnts a dynamic RHI
 */
class RHIContext
{
public:
	RHIContext(GLFWwindow* window)
		: mWindow(window) {};

	virtual bool Init() = 0;
	virtual void Terminate() = 0;

	virtual void SetVsync(bool value) = 0;
	virtual bool GetVsync() const = 0;

	virtual void SetViewport(glm::uvec2 position, glm::uvec2 size) = 0;

	virtual void ClearScreen(bool color, bool depth) = 0;
	virtual void ClearColor(glm::fvec4 color) = 0;

	// drawing functionality
	virtual void DrawMeshWithoutOptimization(Mesh_ptr mesh, Material_ptr material, Camera_ptr camera) = 0;

	// hardware buffers
	virtual RHIVertexBuffer_ptr CreateVertexBuffer(size_t verticesCount, size_t vertexSize, RHIHardwareBufferUsage usage) = 0;
	virtual RHIIndexBuffer_ptr CreateIndexBuffer(size_t indicesCount, RHIIndexBufferType type, RHIHardwareBufferUsage usage) = 0;

	// utilize profiling, swap chain, ...
	virtual void BeginRenderFrame() = 0;
	virtual void EndRenderFrame() = 0;

protected:
	GLFWwindow* mWindow;
};

#endif // ENGINE_RHI_RHICONTEXT_H
