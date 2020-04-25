
#ifndef ENGINE_OPENGL3RHI_OPENGL3RHIBUFFERIMPL_H
#define ENGINE_OPENGL3RHI_OPENGL3RHIBUFFERIMPL_H

#include "../RHI/RHIBuffer.h"

class OpenGl3RHIBufferImpl : public RHIHardwareBuffer
{
public:
	OpenGl3RHIBufferImpl(GLenum target, RHIHardwareBufferUsage usage, size_t size);
	OpenGl3RHIBufferImpl(OpenGl3RHIBufferImpl&) = delete;

	void ReadData(size_t size, size_t offset, void* dest) override final;
	void WriteData(void* src, size_t size, size_t offset, bool discard) override final;

private:
	void Bind();

	GLenum mGlUsage;
	GLenum mTarget;
	GLuint mBufferId;
};

#endif // ENGINE_OPENGL3RHI_OPENGL3RHIBUFFERIMPL_H