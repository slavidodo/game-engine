
#ifndef TESTS_EXAMPLE_SCENE_H
#define TESTS_EXAMPLE_SCENE_H

#include "../Engine/Scene/Scene.h"
#include "../Engine/Core/StaticMesh.h"
#include "../Engine/Rendering/ShaderParameterMacros.h"

__declspec(align(SHADER_PARAMETER_STRUCT_ALIGNMENT))
class PrimitiveUniformShaderParameters
{
public:
	PrimitiveUniformShaderParameters()
		: LocalToWorld(1.0f)
	{}

	TShaderParameterTypeInfo<glm::fmat4x4>::TAlignedType LocalToWorld;

	// TODO we don't wanna bother actually calculating the alignment for members
	// for simplicity and the existence of a single member only, we do it like that
	// but later on, we should calculate the layout and store it in a map identified
	// by the name of the parameters struct
	static RHIUniformBufferLayout& GetLayout() {
		static RHIUniformBufferLayout Layout;
		if (Layout.Resources.size() == 0) {
			// TODO(slavi) should be calculated automatically
			Layout.ConstantBufferSize = 16 * sizeof(float);
			Layout.Resources.push_back({ 0, UBMT_FLOAT32 });
		}

		return Layout;
	}
};

struct PrimitiveSceneElement
{
	Transform_ptr Transform;
	StaticMesh_ptr Mesh;
};

class ExampleScene : public Scene
{
public:
	ExampleScene();

	void Render(RHICommandList& RHICmdList) override final;

private:
	/**
	 * Initialize materials, shaders, render passes
	 */
	void InitGraphcisPipeline();

	/**
	 * Perform any rendering
	 */
	void RenderSceneElements(RHICommandList& RHICmdList);

	std::vector<PrimitiveSceneElement> mSceneElements;

	Camera_ptr mCamera;

	RHIVertexShaderRef mVertexShaderRHI;
	RHIPixelShaderRef mPixelShaderRHI;
	RHIBoundShaderStateRef mBoundShaderState;

	UniformBufferRef<PrimitiveUniformShaderParameters> mUniformBuffer;

	double mLastTime = 0.0;
	float mDeltaTime = 0.0;
};

#endif // TESTS_EXAMPLE_SCENE