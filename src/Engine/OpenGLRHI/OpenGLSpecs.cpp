
#include "Pch.h"

#include "openglspecs.h"

bool OpenGLSpecs::supportsSeparateShaderObjects = false;
bool OpenGLSpecs::supportsVertexAttribBinding = false;
bool OpenGLSpecs::supportsBufferStorage = false;
bool OpenGLSpecs::supportsTessellation = false;

GLint OpenGLSpecs::MaxVertexUniformComponents = -1;
GLint OpenGLSpecs::MaxPixelUniformComponents = -1;
GLint OpenGLSpecs::MaxGeometryUniformComponents = -1;
GLint OpenGLSpecs::MaxHullUniformComponents = -1;
GLint OpenGLSpecs::MaxDomainUniformComponents = -1;

int32_t OpenGLSpecs::MajorVersion = 0;
int32_t OpenGLSpecs::MinorVersion = 0;

namespace {

bool GetGLVersion(int32_t* MajorVersion, int32_t* MinorVersion)
{
	// format '%d.%d.%d - Build %d.%d.%d.%d'
	std::string VersionStr = (const char*)glGetString(GL_VERSION);

	// obtain major version
	size_t pos = VersionStr.find_first_of('.');
	if (pos == std::string::npos) {
		return false;
	}

	std::string MajorVersionStr = VersionStr.substr(0, pos);

	// obtain minor version
	pos = VersionStr.find_first_of('.', pos + 1);
	if (pos == std::string::npos) {
		return false;
	}

	size_t offset = MajorVersionStr.length() + 1;
	std::string MinorVersionStr = VersionStr.substr(offset, pos - offset);

	*MajorVersion = std::atoi(MajorVersionStr.c_str());
	*MinorVersion = std::atoi(MinorVersionStr.c_str());

	return true;
}

}

bool OpenGLSpecs::Init()
{
	if (!GetGLVersion(&MajorVersion, &MinorVersion)) {
		return false;
	}

	supportsSeparateShaderObjects = GLEW_ARB_separate_shader_objects || MajorVersion >= 4;
	supportsVertexAttribBinding = GLEW_VERSION_4_3 || GLEW_ARB_vertex_attrib_binding;
	supportsBufferStorage = GLEW_VERSION_4_3 || GLEW_ARB_vertex_attrib_binding;
	supportsTessellation = GLEW_ARB_tessellation_shader || MajorVersion >= 4;

	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &MaxVertexUniformComponents);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &MaxPixelUniformComponents);
	glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &MaxGeometryUniformComponents);

	if (supportsTessellation) {
		glGetIntegerv(GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS, &MaxHullUniformComponents);
		glGetIntegerv(GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS, &MaxDomainUniformComponents);
	} else {
		MaxHullUniformComponents = 0;
		MaxDomainUniformComponents = 0;
	}

	return true;
}