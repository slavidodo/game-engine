
#ifndef ENGINE_OPENGLRHI_OPENGLSPECS_H
#define ENGINE_OPENGLRHI_OPENGLSPECS_H

class OpenGLSpecs
{
public:
	enum class GLResourceLockMode : uint8_t
	{
		RLM_ReadWrite,
		RLM_ReadOnly,
		RLM_WriteOnly,
		RLM_WriteOnlyUnsynchronized,
		RLM_WriteOnlyPersistent,
	};

	static bool Init();

	static inline bool SupportsUniformBuffers() { return true; }
	static inline bool SupportsSeparateShaderObjects() { return supportsSeparateShaderObjects; }
	static inline bool SupportsVertexAttribBinding() { return supportsVertexAttribBinding; }
	static inline bool SupportsBufferStorage() { return supportsBufferStorage; }
	static inline bool SupportsTessellation() { return supportsTessellation; }

	static inline GLint GetMaxPixelUniformComponents() { return MaxPixelUniformComponents; }
	static inline GLint GetMaxVertexUniformComponents() { return MaxVertexUniformComponents; }
	static inline GLint GetMaxGeometryUniformComponents() { return MaxGeometryUniformComponents; }
	static inline GLint GetMaxHullUniformComponents() { return MaxHullUniformComponents; }
	static inline GLint GetMaxDomainUniformComponents() { return MaxDomainUniformComponents; }
	static inline GLint GetMaxComputeUniformComponents() { return 0; }

	// OpenGL functions
	static inline void GenVertexArrays(GLsizei N, GLuint* Arrays) {
		glGenVertexArrays(N, Arrays);
	}
	static inline void DeleteVertexArrays(GLsizei N, GLuint* Arrays) {
		glDeleteVertexArrays(N, Arrays);
	}
	static inline void BindVertexArray(GLuint Array) {
		glBindVertexArray(Array);
	}

	static inline void GenBuffers(GLsizei N, GLuint* Buffers) {
		glGenBuffers(N, Buffers);
	}
	static inline void DeleteBuffers(GLsizei N, GLuint* Buffers) {
		glGenBuffers(N, Buffers);
	}
	static inline void BindBuffer(GLenum Target, GLuint Buffer) {
		glBindBuffer(Target, Buffer);
	}

	static inline void BufferStorage(GLenum Target, GLsizeiptr Size, const void* Data, GLbitfield Flags) {
		glBufferStorage(Target, Size, Data, Flags);
	}

	static inline void* MapBufferRange(GLenum Type, GLintptr MemberOffset, GLsizeiptr Size, GLResourceLockMode LockMode) {
		GLenum Access;
		switch (LockMode) {
			case GLResourceLockMode::RLM_ReadOnly:
				Access = GL_MAP_READ_BIT;
				break;
			case GLResourceLockMode::RLM_WriteOnly:
				Access = (GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT);
#if 1
				// Temp workaround for synchrnoization when a UBO is discarded while being referenced
				Access |= GL_MAP_UNSYNCHRONIZED_BIT;
#endif
				break;
			case GLResourceLockMode::RLM_WriteOnlyUnsynchronized:
				Access = (GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
				break;
			case GLResourceLockMode::RLM_WriteOnlyPersistent:
				Access = (GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
				break;
			case GLResourceLockMode::RLM_ReadWrite:
			default:
				Access = (GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
		}
		return glMapBufferRange(Type, MemberOffset, Size, Access);
	}
	static inline void UnmapBuffer(GLenum Type) {
		glUnmapBuffer(Type);
	}

private:
	static bool supportsSeparateShaderObjects;
	static bool supportsVertexAttribBinding;
	static bool supportsBufferStorage;
	static bool supportsTessellation;

	static GLint MaxVertexUniformComponents;
	static GLint MaxPixelUniformComponents;
	static GLint MaxGeometryUniformComponents;
	static GLint MaxHullUniformComponents;
	static GLint MaxDomainUniformComponents;

	static int32_t MajorVersion;
	static int32_t MinorVersion;
};

#endif // ENGINE_OPENGLRHI_OPENGLSPECS_H