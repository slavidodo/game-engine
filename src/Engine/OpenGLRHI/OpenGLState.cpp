
#include "pch.h"

#include "OpenGLState.h"

void OpenGLRHIState::InitializeResources()
{
	OpenGLCommonState::InitializeResources();

	ShaderParameters = new OpenGLShaderParameterCache[NUM_SHADER_STAGES];
	ShaderParameters[SHADER_STAGE_VERTEX].InitializeResources(OpenGLSpecs::GetMaxVertexUniformComponents() * 4 * sizeof(float));
	ShaderParameters[SHADER_STAGE_PIXEL].InitializeResources(OpenGLSpecs::GetMaxPixelUniformComponents() * 4 * sizeof(float));
	ShaderParameters[SHADER_STAGE_GEOMETRY].InitializeResources(OpenGLSpecs::GetMaxGeometryUniformComponents() * 4 * sizeof(float));

	if (OpenGLSpecs::SupportsTessellation()) {
		ShaderParameters[SHADER_STAGE_HULL].InitializeResources(OpenGLSpecs::GetMaxHullUniformComponents() * 4 * sizeof(float));
		ShaderParameters[SHADER_STAGE_DOMAIN].InitializeResources(OpenGLSpecs::GetMaxDomainUniformComponents() * 4 * sizeof(float));
	}

	LinkedProgramAndDirtyFlag = nullptr;

	for (int32_t Frequency = 0; Frequency < SF_NumStandardFrequencies; ++Frequency) {
		DirtyUniformBuffers[Frequency] = std::numeric_limits<int16_t>::max();
	}
	AnyDirtyUniformBuffers = true;
}

void OpenGLRHIState::CleanupResources()
{
	delete[] ShaderParameters;
	ShaderParameters = NULL;

	// Release references to bound uniform buffers.
	for (int32_t Frequency = 0; Frequency < SF_NumStandardFrequencies; ++Frequency) {
		for (int32_t BindIndex = 0; BindIndex < MAX_UNIFORM_BUFFERS_PER_SHADER_STAGE; ++BindIndex) {
			BoundUniformBuffers[Frequency][BindIndex].SafeRelease();
		}
	}

	OpenGLCommonState::CleanupResources();
}