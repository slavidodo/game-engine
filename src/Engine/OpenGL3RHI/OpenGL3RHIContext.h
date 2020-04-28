
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

	RHIVertexBuffer_ptr CreateVertexBuffer(size_t verticesCount, size_t vertexSize, RHIHardwareBufferUsage usage) override final;
	void* LockVertexBuffer(RHIVertexBuffer_ptr vertexBuffer, size_t offset, size_t size, RHIResourceLockMode lockMode) override final;
	void UnlockVertexBuffer(RHIVertexBuffer_ptr vertexBuffer) override final;
	void CopyVertexBuffer(RHIVertexBuffer_ptr sourceBuffer, RHIVertexBuffer_ptr destBuffer) override final;

	RHIIndexBuffer_ptr CreateIndexBuffer(size_t indicesCount, RHIIndexBufferType type, RHIHardwareBufferUsage usage) override final;
	void* LockIndexBuffer(RHIIndexBuffer_ptr indexBuffer, size_t offset, size_t size, RHIResourceLockMode lockMode) override final;
	void UnlockIndexBuffer(RHIIndexBuffer_ptr indexBuffer) override final;

	void BeginRenderFrame() override final;
	void EndRenderFrame() override final;

	GLenum GetGLUsageForHardwareUsage(RHIHardwareBufferUsage hbUsage);

private:
	GLFWwindow* mContext;

	bool mVsync;
};

#endif // ENGINE_OPENGL3RHI_OPENGL3RHICONTEXT_H
