
#include "pch.h"

#include "ExampleScene.h"

#include "../Engine/StaticMeshGenerator.h"

#include "../Engine/Filesystem/ResourceManager.h"

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

void AddPlayer(Transform_ptr transform, glm::fvec3 color) {
	glm::fvec3 halfDimensions = transform->GetScale() * 0.5f;
	RenderActor_ptr renderActor = std::make_shared<RenderActor>(StaticMeshGenerator::CreateBox(halfDimensions * 0.00001f, color));

	PhysicsActor_ptr physicsActor = PDynamicActor::CreateActor(transform);
	physicsActor->AddCollider(PCollider::CreateCollider(PBoxGeometry::CreateGeometry(halfDimensions), PMaterial::CreateMaterial(100.0f, 100.0f, 0.3f)));

	Actor_ptr player = std::make_shared<Actor>(transform, renderActor, physicsActor);
	SceneManager::GetInstance().AddActor(player);
	if (PhysicsActor_ptr physicsActor = player->mPhysicsActor.lock()) {
		PDynamicActor_ptr dynamicPlayer = std::static_pointer_cast<PDynamicActor>(physicsActor);
		dynamicPlayer->LockMotion(PDynamicActor::MotionAxis::ANGULAR_Z);
		dynamicPlayer->LockMotion(PDynamicActor::MotionAxis::ANGULAR_X);
		SceneManager::GetInstance().SetPlayer(player);
	}
}
void AddBoxActor(bool bStatic, Transform_ptr transform, glm::fvec3 color) {
	glm::fvec3 halfDimensions = transform->GetScale() * 0.5f;
	RenderActor_ptr renderActor = std::make_shared<RenderActor>(StaticMeshGenerator::CreateBox(halfDimensions * 0.55f, color));
	
	PhysicsActor_ptr physicsActor = nullptr;
	if (bStatic) physicsActor = PStaticActor::CreateActor(transform);
	else         physicsActor = PDynamicActor::CreateActor(transform);
	physicsActor->AddCollider(PCollider::CreateCollider(PBoxGeometry::CreateGeometry(halfDimensions), PMaterial::CreateMaterial(100.0f, 100.0f, 0.3f)));

	SceneManager::GetInstance().AddActor(std::make_shared<Actor>(transform, renderActor, physicsActor));
}
void RunExampleScene() {
	bool bStatic = true;
	bool bDynamic = false;

	/// Floor
	AddBoxActor(bStatic, std::make_shared<Transform>(glm::fvec3(0.0f, 0.0f, 0.0f), glm::fvec3(0.0f), glm::fvec3(100.0f, 1.0f, 100.0f)), glm::fvec3(0.0f));

	/// Platforms
	AddBoxActor(bStatic, std::make_shared<Transform>(glm::fvec3(0.0f, 2.0f, 0.0f), glm::fvec3(0.0f), glm::fvec3(2.0f, 0.2f, 2.0f)), glm::fvec3(0.0f, 0.0f, 1.0f));
	AddBoxActor(bStatic, std::make_shared<Transform>(glm::fvec3(-4.0f, 4.0f, 0.0f), glm::fvec3(0.0f), glm::fvec3(2.0f, 0.2f, 2.0f)), glm::fvec3(1.0f, 0.0f, 0.0f));
	AddBoxActor(bStatic, std::make_shared<Transform>(glm::fvec3(-8.0f, 6.0f, 0.0f), glm::fvec3(0.0f), glm::fvec3(2.0f, 0.2f, 2.0f)), glm::fvec3(0.0f, 1.0f, 0.0f));
	AddBoxActor(bStatic, std::make_shared<Transform>(glm::fvec3(-8.0f, 8.0f, -4.0f), glm::fvec3(0.0f), glm::fvec3(2.0f, 0.2f, 2.0f)), glm::fvec3(1.0f, 0.0f, 1.0f));
	
	//AddBoxActor(bDynamic, std::make_shared<Transform>(glm::fvec3(0.0f, 1.0f, -2.0f), glm::fvec3(0.0f), glm::fvec3(0.5f)),             glm::fvec3(1.0f, 1.0f, 0.0f));
	AddPlayer(            std::make_shared<Transform>(glm::fvec3(2.0f, 5.0f, 2.0f),  glm::fvec3(0.0f), glm::fvec3(0.3f, 0.3f, 0.3f)), glm::fvec3(0.0f, 1.0f, 1.0f));
}



ExampleScene::ExampleScene(RenderScene_ptr renderScene, PhysicsScene_ptr physicsScene) : Scene(renderScene, physicsScene) {
	mRenderScene->mRenderFunction = std::bind(&ExampleScene::Render, this, std::placeholders::_1);
	mPhysicsScene->mUpdateFunction = std::bind(&ExampleScene::UpdatePhysics, this);

	mRenderScene->mMainCamera = Camera::CreatePerspectiveCamera(45.0f, 0.1f, 1000.f);
	mRenderScene->mMainCamera->SetLocalTranslation(glm::fvec3(0.0f, 0.15f, 0.0f));
	
	//mRenderScene->mMainCamera->SetEditorTranslation(glm::fvec3(0.0f, 5.0f, 20.0f));
	//mRenderScene->mMainCamera->SetEditorTranslation(glm::fvec3(0.0f, 30.0f, 3.0f));
	//mRenderScene->mMainCamera->SetEditorRotationDegrees(glm::fvec3(-30.0f, 0.0f, 0.0f));

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
	mRenderScene->mVertexShaderRHI = OpenGLShaderCompiler::ManuallyCreateVertexShader(RHICmdList, VertexShaderBytes, 1);
	mRenderScene->mPixelShaderRHI = OpenGLShaderCompiler::ManuallyCreatePixelShader(RHICmdList, PixelShaderBytes, 0);
	mRenderScene->mBoundShaderState = RHICmdList.CreateBoundShaderState(gStaticMeshVertexDeclaration.VertexDeclarationRHI, mRenderScene->mVertexShaderRHI, mRenderScene->mPixelShaderRHI);
	
	mRenderScene->mUniformBuffer = UniformBufferRef<PrimitiveUniformShaderParameters>::CreateUniformBufferImmediate(
		PrimitiveUniformShaderParameters(), EUniformBufferUsage::UniformBuffer_MultiFrame);

	gDynamicRHI->RHISetShaderUniformBuffer(mRenderScene->mVertexShaderRHI, 0, mRenderScene->mUniformBuffer);
}

void ExampleScene::UpdatePhysics() {
	double currentTime = glfwGetTime(); // really?
	mDeltaTime = static_cast<float>(currentTime - mLastTime);
	mLastTime = currentTime;

	mPhysicsScene->Update(mDeltaTime);
	for (Actor_ptr actor : mActors) actor->UpdateTransformsFromPhysics();
}

void ExampleScene::Render(RHICommandList& RHICmdList)
{
	// Let the RHI know that we are drawing a scene
	// the main use of this is profiling on a dev build
	RHICmdList.BeginScene();
	RenderSceneActors(RHICmdList);
	RHICmdList.EndScene();
}
void ExampleScene::RenderSceneActors(RHICommandList& RHICmdList)
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
		RHICmdList.SetBoundShaderState(mRenderScene->mBoundShaderState);

		Camera_ptr camera = mRenderScene->mMainCamera;
		glm::mat4x4 ViewProjection = camera->GetProjMatrix() * camera->GetViewMatrix();

		for (Actor_ptr actor : mActors) {
			RenderActor_ptr renderActor = actor->mRenderActor.lock();
			if (!renderActor) continue;
			Transform_ptr transform = renderActor->mTransform;
			StaticMesh_ptr mesh = renderActor->mMesh;

			// set the vertex buffer to be our data source
			RHICmdList.SetStreamSource(0, mesh->VertexBuffer, 0);

			// make sure all matricies are calculated if any changes occurred
			transform->EnsureUpdated();

			// Update shader paramaters
			PrimitiveUniformShaderParameters Parameters;
			Parameters.LocalToWorld = ViewProjection * transform->GetLocalToWorld();
			mRenderScene->mUniformBuffer.UpdateUniformBufferImmediate(Parameters);

			// this is yet fixed to draw triangles, in some cases or might be preffered
			// by the user, we may need to determine the topology
			// this will issue a draw call with index buffer
			RHICmdList.DrawIndexedPrimitive(mesh->IndexBuffer, 0, 0, mesh->NumVertices, 0, mesh->NumTriangles, 1);
		}
		modelTest->RenderSceneElements(RHICmdList);
	}
	RHICmdList.EndRenderPass();
}