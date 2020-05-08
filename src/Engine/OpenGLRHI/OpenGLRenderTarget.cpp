
#include "pch.h"

#include "OpenGLDynamicRHI.h"

#include "../Platform/Window.h"

void OpenGLDynamicRHI::BindPendingFramebuffer(OpenGLContextState& ContextState)
{
	if (ContextState.Framebuffer != mPendingState.Framebuffer) {
		glBindFramebuffer(GL_FRAMEBUFFER, mPendingState.Framebuffer);
		if (mPendingState.Framebuffer != 0) {
			// TODO; multiple render targets
			// just use an array of render targets
			// and if any updates needed call glDrawBuffers(n, bufs)
		} else {
			glReadBuffer(GL_BACK);
			glDrawBuffer(GL_BACK);
		}
	}
}

void OpenGLDynamicRHI::RHIBeginRenderPass(RHIRenderPassInfo& Info)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	ClearColor(glm::fvec4(1.0f, 1.0f, 0.0f, 1.0f));
}

void OpenGLDynamicRHI::RHIEndRenderPass()
{
	// swap buffers
	glfwSwapBuffers(gWindow.GetGlfwWindow());
}