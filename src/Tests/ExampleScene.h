
#ifndef TESTS_EXAMPLE_SCENE_H
#define TESTS_EXAMPLE_SCENE_H

#include "../Engine/Scene/Scene.h"
#include "../Engine/Core/StaticMesh.h"

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

	void RenderCustom(RHICommandList& RHICmdList);

	Transform_ptr mTransform;
	StaticMesh_ptr mStaticMesh;
	StaticMesh_ptr mStaticMesh2;

	RHIVertexBuffer_ptr mVertexBufferRHI;
	RHIIndexBuffer_ptr mIndexBufferRHI;

	RHIBoundShaderState_ptr mBoundShaderState;
};

#endif // TESTS_EXAMPLE_SCENE