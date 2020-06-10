
#include "pch.h"

#include "ExampleScene.h"

#include "../Engine/StaticMeshGenerator.h"

// TODO: we should not include any opengl specific headers
// but for now in a test-phase we do so
#include "../Engine/OpenGLRHI/OpenGLShaderCompiler.h"

const std::string SimpleVertexShaderCode = "\
#version 330 core\n\
layout (location = 0) in vec3 inPositionfa;\n\
layout (location = 1) in vec3 inColor;\n\
layout (location = 2) in vec2 inUV;\n\
out vec4 vsColor;\n\
layout(std140) uniform vb0 {\n\
	highp mat4 mvp;\n\
};\n\
void main() {\n\
	gl_Position = mvp * vec4(inPositionfa, 1.0f);\n\
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
		Elements.push_back(VertexElement(0, offsetof(StaticMeshVertex, color), VET_Float3, 1, Stride));
		Elements.push_back(VertexElement(0, offsetof(StaticMeshVertex, uv), VET_Float2, 2, Stride));
		VertexDeclarationRHI = gDynamicRHI->RHICreateVertexDeclaration(Elements);
	}

	RHIVertexDeclarationRef VertexDeclarationRHI;
};

// TODO(slavi) these shouldn't be there for a single scene
// instead use this as a general vertex declaration
static StaticMeshVertexDeclaration gStaticMeshVertexDeclaration;

ExampleScene::ExampleScene()
{
	// Create the physics scene
	PSceneManager::GetInstance().SetCurrentScene(PScene::CreateScene(PSceneDescriptor()));

	PBoxGeometry_ptr geometry = PBoxGeometry::CreateGeometry(glm::vec3(0.5f, 0.5f, 0.5f));

	mSceneElements.push_back({
		std::make_shared<Transform>(
			glm::fvec3(-1.5f, 0, -4.5f),
			glm::radians(glm::fvec3(0.f, -45.f, 0.f)),
			glm::fvec3(1)),
		nullptr,
	});
	PStaticActor_ptr physicsActor1 = PStaticActor::CreateActor(glm::fvec3(-1.5f, 0, -4.5f), glm::radians(glm::fvec3(0.f, -45.f, 0.f)));
	physicsActor1->AddCollider(PCollider::CreateCollider(geometry));
	PSceneManager::GetInstance().AddActor(physicsActor1);


	mSceneElements.push_back({
		std::make_shared<Transform>(
			glm::fvec3(1.5f, 0, -4.5f),
			glm::radians(glm::fvec3(0.f, 45.f, 0.f)),
			glm::fvec3(1)),
		nullptr,
	});
	PStaticActor_ptr physicsActor2 = PStaticActor::CreateActor(glm::fvec3(-1.5f, 0, -4.5f), glm::radians(glm::fvec3(0.f, -45.f, 0.f)));
	physicsActor2->AddCollider(PCollider::CreateCollider(geometry));
	PSceneManager::GetInstance().AddActor(physicsActor2);


	mCamera = Camera::CreatePerspectiveCamera(45.0f, 0.1f, 1000.f);
	mCamera->SetEditorTranslation(glm::fvec3(0, 10.f, 1.f));
	mCamera->SetEditorRotationDegrees(glm::fvec3(-35.f, 0, 0.0f));

	InitGraphcisPipeline();
}

void ExampleScene::InitGraphcisPipeline()
{
	// this is fine to do, as we are creating things, an immediate context
	// must be present
	RHICommandListImmediate& RHICmdList = RHICommandListExecutor::GetImmediateCommandList();

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

	// UPDATE: now that we need to actually need to bind cbuffers to
	// the shader, we need to manually add information about them
	// later on, there should be a dedicated shader compiler to generate
	// them automatically and obtain their locations (opengl)
	// for directx and vulkan, the shader is compiled at compile-time
	// using the editor and the information is stored along with it
	mVertexShaderRHI = OpenGLShaderCompiler::ManuallyCreateVertexShader(RHICmdList, VertexShaderBytes, 1);
	mPixelShaderRHI = OpenGLShaderCompiler::ManuallyCreatePixelShader(RHICmdList, PixelShaderBytes, 0);
	mBoundShaderState = RHICmdList.CreateBoundShaderState(
		gStaticMeshVertexDeclaration.VertexDeclarationRHI,
		mVertexShaderRHI,
		mPixelShaderRHI);

	// finally, create some fancy meshes to draw
	mSceneElements[0].Mesh = StaticMeshGenerator::CreateCube(1.0f);
	mSceneElements[1].Mesh = StaticMeshGenerator::CreateCube(1.0f);

	mUniformBuffer = UniformBufferRef<PrimitiveUniformShaderParameters>::CreateUniformBufferImmediate(
		PrimitiveUniformShaderParameters(), EUniformBufferUsage::UniformBuffer_MultiFrame);

	gDynamicRHI->RHISetShaderUniformBuffer(mVertexShaderRHI, 0, mUniformBuffer);
}

void ExampleScene::Render(RHICommandList& RHICmdList)
{
	double currentTime = glfwGetTime(); // really?
	mDeltaTime = static_cast<float>(currentTime - mLastTime);
	mLastTime = currentTime;

	// Let the RHI know that we are drawing a scene
	// the main use of this is profiling on a dev build
	RHICmdList.BeginScene();
	{
		RenderSceneElements(RHICmdList);
	}
	RHICmdList.EndScene();

	/// Update the physics actors
	PSceneManager::GetInstance().Update(mDeltaTime);
	PSceneManager::GetInstance().ApplyUpdateResults(true);

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
		// TODO: should call SetGraphicsPSO instead
		// setting shader should only Update once since we're not really changing the shaders
		// between frames/draw calls
		RHICmdList.SetBoundShaderState(mBoundShaderState);

		glm::mat4x4 ViewProjection = mCamera->GetProjMatrix() * mCamera->GetViewMatrix();

		for (PrimitiveSceneElement& SceneElement : mSceneElements) {
			Transform_ptr Transform = SceneElement.Transform;
			StaticMesh_ptr Mesh = SceneElement.Mesh;

			// set the vertex buffer to be our data source
			RHICmdList.SetStreamSource(0, Mesh->VertexBuffer, 0);

			// make sure all matricies are calculated if any changes occurred
			Transform->EnsureUpdated();

			// Update shader paramaters
			PrimitiveUniformShaderParameters Parameters;
			Parameters.LocalToWorld = ViewProjection * SceneElement.Transform->GetLocalToWorld();
			mUniformBuffer.UpdateUniformBufferImmediate(Parameters);

			// this is yet fixed to draw triangles, in some cases or might be preffered
			// by the user, we may need to determine the topology
			// this will issue a draw call with index buffer
			RHICmdList.DrawIndexedPrimitive(Mesh->IndexBuffer, 0, 0, Mesh->NumVertices, 0, Mesh->NumTriangles, 1);
		}
	}
	RHICmdList.EndRenderPass();
}