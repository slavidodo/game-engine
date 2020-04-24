
#ifndef ENGINE_OPENGL3RHI_OPENGL3RHICONTEXT_H
#define ENGINE_OPENGL3RHI_OPENGL3RHICONTEXT_H

#include "../RHI/RHIContext.h"

class OpenGL3RHIContext : public RHIContext
{
public:
	OpenGL3RHIContext(GLFWwindow* window);

	bool Init() override final;
	void Terminate() override final;

	bool GetVsync() const { return mVsync; }
	void SetVsync(bool value) {
		mVsync = value;
		glfwSwapInterval(mVsync);
	}

	void SetViewport(glm::uvec2 position, glm::uvec2 size) override final;

	void ClearScreen(bool color, bool depth) override final;
	void ClearColor(glm::fvec4 color) override final;

	void DrawMeshWithoutOptimization(Mesh_ptr mesh, Material_ptr shader, Camera_ptr camera) override final;

	RHIVertexBuffer_ptr CreateVertexBuffer(size_t verticesCount, size_t vertexSize, RHIHardwareBufferUsage usage) override final {
		return nullptr;
	}
	RHIIndexBuffer_ptr CreateIndexBuffer(size_t indicesCount, RHIIndexBufferType type, RHIHardwareBufferUsage usage) override final {
		return nullptr;
	}

	void BeginRenderFrame() override final;
	void EndRenderFrame() override final;

private:
	GLFWwindow* mContext;

	bool mVsync;
};

#endif // ENGINE_OPENGL3RHI_OPENGL3RHICONTEXT_H
