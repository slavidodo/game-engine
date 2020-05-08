
#include "pch.h"

#include "ExampleScene.h"

#include "../Engine/StaticMeshGenerator.h"

const std::string SimpleVertexShaderCode = "\
#version 330 core\n\
layout (location = 0) in vec3 inPosition;\n\
layout (location = 1) in vec3 inColor;\n\
layout (location = 2) in vec2 inUV;\n\
out vec4 vsColor;\n\
void main() {\n\
	gl_Position = vec4(inPosition, 1.0f);\n\
	vsColor = vec4(inColor, 1.0f);\n\
}";

const std::string SimplePixelShaderCode = "\
#version 330 core\n\
out vec4 outColor;\n\
in vec4 vsColor;\n\
void main() {\n\
	outColor = vsColor;\n\
}\0";

class StaticMeshVertexDeclaration
{
public:
	void InitRHI() {
		VertexDeclarationElementList Elements;
		uint16_t Stride = sizeof(VertexDeclarationElementList);
		Elements.push_back(VertexElement(0, offsetof(StaticMeshVertex, position), VET_Float3, 0, Stride));
		Elements.push_back(VertexElement(0, offsetof(StaticMeshVertex, normal), VET_Float3, 1, Stride));
		Elements.push_back(VertexElement(0, offsetof(StaticMeshVertex, uv), VET_Float2, 2, Stride));
		VertexDeclarationRHI = gDynamicRHI->RHICreateVertexDeclaration(Elements);
	}

	RHIVertexDeclaration_ptr VertexDeclarationRHI;
};

// TODO:
// this shouldn't be there for a single scene
// instead use this as a general vertex declaration
static StaticMeshVertexDeclaration gStaticMeshVertexDeclaration;

ExampleScene::ExampleScene()
{
	mTransform = Transform_ptr(new Transform(glm::fvec3(0), glm::fvec3(0), glm::fvec3(1)));
	

	InitGraphcisPipeline();
}

void ExampleScene::InitGraphcisPipeline()
{
	// this is fine to do, as we are creating things, an immediate context
	// must be present
	auto& RHICmdList = RHICommandListExecutor::GetImmediateCommandList();

	// vertex declaration; any render resource must be initialized
	// TODO: move it to batched elements where the actual draw happens
	gStaticMeshVertexDeclaration.InitRHI();

	// TODO: this is hardcoded for opengl only, we should use a
	// shader manager to fetch this from resources or at least
	// move it out of here and load it using resource manager
	std::vector<uint8_t> VertexShaderBytes, PixelShaderBytes;
	std::copy(SimpleVertexShaderCode.begin(), SimpleVertexShaderCode.end(), std::back_inserter(VertexShaderBytes));
	std::copy(SimplePixelShaderCode.begin(), SimplePixelShaderCode.end(), std::back_inserter(PixelShaderBytes));

	// compile shaders and obtain a bound shader state
	// i suggest the name here NOT to be "bound" since this shader
	// is not actually bound unless the RHI decides to
	// but we call it bound here since we ONLY 
	auto VertexShaderRHI = RHICmdList.CreateVertexShader(VertexShaderBytes);
	auto PixelShaderRHI = RHICmdList.CreatePixelShader(PixelShaderBytes);
	mBoundShaderState = RHICmdList.CreateBoundShaderState(
		gStaticMeshVertexDeclaration.VertexDeclarationRHI,
		VertexShaderRHI,
		PixelShaderRHI);

	// finally, create some fancy triangle to be drawn later
	mStaticMesh = StaticMeshGenerator::CreateTriangle(1.0f);
}

void ExampleScene::Render(RHICommandList& RHICmdList)
{
	// Let the RHI know that we are drawing a scene
	// the main use of this is actually profiling on a dev build
	RHICmdList.BeginScene();
	{
		RenderSceneElements(RHICmdList);
	}
	RHICmdList.EndScene();
}

void ExampleScene::RenderSceneElements(RHICommandList& RHICmdList)
{
	// RHI should take care for our context VAO (in opengl)
	// we shoul still pass shader

	// todo; for now we are not using a framebuffer (bound by RHITexture)
	// the RHI will assume this is intended and will use the default back
	// buffer
	RHIRenderPassInfo RPInfo; // { Ref<RHITexture> ColorRT }
	RHICmdList.BeginRenderPass(RPInfo);
	{
		// TODO: should call SETGraphicsPSO instead
		// Set bound shader, should only update once since we're not really changing
		RHICmdList.SetBoundShaderState(mBoundShaderState);

		// set the vertex buffer to be our data source
		RHICmdList.SetStreamSource(0, mStaticMesh->VertexBuffer, 0);

		// this is yet fixed to draw triangles, in some cases or might be preffered
		// by the user, we may need to determine the toplogy
		// this will issue a draw call with index buffer
		RHICmdList.DrawIndexedPrimitive(mStaticMesh->IndexBuffer, 0, 0, mStaticMesh->NumVertices, 0, mStaticMesh->NumTriangles, 1);
	}
	RHICmdList.EndRenderPass();
}