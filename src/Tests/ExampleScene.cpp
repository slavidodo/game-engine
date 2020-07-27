
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



const std::string SimpleLightedVertexShaderCode = "\
#version 330 core\n\
layout (location = 0) in vec3 inPositionfa;\n\
layout (location = 1) in vec3 inNormal;\n\
layout (location = 2) in vec3 inColor;\n\
layout (location = 3) in vec2 inUV;\n\
out vec3 vsColor;\n\
out vec3 vsNormal;\n\
out vec2 vsUV;\n\
out vec3 fragPos;\n\
uniform mat4 mvp;\n\
uniform mat4 modelMat;\n\
void main() {\n\
	gl_Position = mvp * vec4(inPositionfa, 1.0f);\n\
	vsColor = inColor;\n\
	vsUV = inUV;\n\
	mat3 normalMat = mat3(transpose(inverse(modelMat)));\n\
	vsNormal = normalMat * inNormal;\n\
	fragPos = mat3(modelMat) * inPositionfa;\n\
}";

const std::string SimpleLightedPixelShaderCode = "\
#version 330 core\n\
out vec4 outColor;\n\
in vec3 vsColor;\n\
in vec3 vsNormal;\n\
in vec2 vsUV;\n\
in vec3 fragPos;\n\
uniform sampler2D tex;\n\
void main() {\n\
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);\n\
	vec3 lightPos = vec3(12.0f, 12.0f, 12.0f);\n\
	float d = length(fragPos - lightPos);\n\
	float attenuation = 14.0f / (1.0f + d * 0.09f + d * d * 0.03f);\n\
	vec3 ambient = 0.4f * lightColor;\n\
	vec3 norm = normalize(vsNormal);\n\
	vec3 lightDir = normalize(lightPos - fragPos);\n\
	float diff = max(dot(norm, lightDir), 0.0f);\n\
	vec3 diffuse = 1.6f * diff * lightColor;\n\
	if (d < 3.0f) {\n\
		outColor = vec4(1.0f);\n\
	} else {\n\
		vec3 result = (ambient + diffuse * attenuation) * vec3(texture(tex, vsUV));\n\
		outColor = vec4(result, 1.0f);\n\
	}\n\
}\0";

const std::string SimpleLighterPixelShaderCode = "\
#version 330 core\n\
out vec4 outColor;\n\
in vec3 vsColor;\n\
in vec3 vsNormal;\n\
in vec2 vsUV;\n\
in vec3 fragPos;\n\
uniform sampler2D tex;\n\
void main() {\n\
	outColor = vec4(1.0f);\n\
}\0";



const std::string SimpleModelLoadingVS = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
out vec2 TexCoords;
layout(std140) uniform vb0 {
	highp mat4 mvp;
};
void main(){
    TexCoords = aTexCoords;    
    gl_Position = mvp * vec4(aPos, 1.0);
}
)";


const std::string SimpleModelLoadingFS = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
    //FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
)";


class StaticMeshVertexDeclaration
{
public:
	void InitRHI() {
		VertexDeclarationElementList Elements;
		uint16_t Stride = sizeof(StaticMeshLightedVertex);
		Elements.push_back(VertexElement(0, offsetof(StaticMeshLightedVertex, position), VET_Float3, 0, Stride));
		Elements.push_back(VertexElement(0, offsetof(StaticMeshLightedVertex, normal), VET_Float3, 1, Stride));
		Elements.push_back(VertexElement(0, offsetof(StaticMeshLightedVertex, color), VET_Float3, 2, Stride));
		Elements.push_back(VertexElement(0, offsetof(StaticMeshLightedVertex, uv), VET_Float2, 3, Stride));
		VertexDeclarationRHI = gDynamicRHI->RHICreateVertexDeclaration(Elements);
	}

	RHIVertexDeclarationRef VertexDeclarationRHI;
};
// TODO(slavi) these shouldn't be there for a single scene
// instead use this as a general vertex declaration
static StaticMeshVertexDeclaration gStaticMeshVertexDeclaration;

void AddPlayer(Transform_ptr transform, glm::fvec3 color, Texture_ptr tex) {
	glm::fvec3 halfDimensions = transform->GetScale() * 0.5f;
	RenderActor_ptr renderActor = std::make_shared<RenderActor>(StaticMeshGenerator::CreateLightedBox(halfDimensions * 0.00001f, color));

	PhysicsActor_ptr physicsActor = PDynamicActor::CreateActor(transform);
	physicsActor->AddCollider(PCollider::CreateCollider(PBoxGeometry::CreateGeometry(halfDimensions), PMaterial::CreateMaterial(100.0f, 100.0f, 0.3f)));

	Actor_ptr player = std::make_shared<Actor>(transform, renderActor, physicsActor);
	player->SetTexture(tex);
	SceneManager::GetInstance().AddActor(player);
	
	if (PhysicsActor_ptr physicsActor = player->mPhysicsActor.lock()) {
		PDynamicActor_ptr dynamicPlayer = std::static_pointer_cast<PDynamicActor>(physicsActor);
		dynamicPlayer->LockMotion(PDynamicActor::MotionAxis::ANGULAR_Z);
		dynamicPlayer->LockMotion(PDynamicActor::MotionAxis::ANGULAR_X);
		SceneManager::GetInstance().SetPlayer(player);
	}
}
void AddBoxActor(int state, Transform_ptr transform, glm::fvec3 color, Texture_ptr tex, bool lighter = false, glm::fvec3 lim1 = glm::fvec3(0), glm::fvec3 lim2 = glm::fvec3(0)) {
	glm::fvec3 halfDimensions = transform->GetScale() * 0.5f;
	RenderActor_ptr renderActor = std::make_shared<RenderActor>(StaticMeshGenerator::CreateLightedBox(halfDimensions * 0.55f, color));

	PhysicsActor_ptr physicsActor = nullptr;
	if (!state) physicsActor = PStaticActor::CreateActor(transform);
	else         physicsActor = PDynamicActor::CreateActor(transform);
	physicsActor->AddCollider(PCollider::CreateCollider(PBoxGeometry::CreateGeometry(halfDimensions), PMaterial::CreateMaterial(100.0f, 100.0f, 0.3f)));
	if (state == 2) {
		PDynamicActor_ptr dynamicActor = std::static_pointer_cast<PDynamicActor>(physicsActor);
		dynamicActor->SetKinematic(true);
	}

	Actor_ptr actor = std::make_shared<Actor>(transform, renderActor, physicsActor);
	if (state == 2) {
		actor->kinematic = true;
		actor->limit1 = lim1;
		actor->limit2 = lim2;
		actor->originalPos = transform->GetTranslation();
	}
	actor->SetTexture(tex);
	actor->lighter = lighter;
	SceneManager::GetInstance().AddActor(actor);

}
void AddModelSingleMeshActor(bool bStatic, Transform_ptr transform, std::string path) {
	Model_ptr modelTest = ResourceManager::GetInstance().LoadModel(path);
	if (!modelTest) return;

	glm::fvec3 halfDimensions = transform->GetScale() * 0.5f;
	RenderActor_ptr renderActor = std::make_shared<RenderActor>(modelTest->GetFirstMesh());

	PhysicsActor_ptr physicsActor = nullptr;
	if (bStatic) physicsActor = PStaticActor::CreateActor(transform);
	else         physicsActor = PDynamicActor::CreateActor(transform);
	physicsActor->AddCollider(PCollider::CreateCollider(PBoxGeometry::CreateGeometry(halfDimensions), PMaterial::CreateMaterial(100.0f, 100.0f, 0.3f)));

	SceneManager::GetInstance().AddActor(std::make_shared<Actor>(transform, renderActor, physicsActor, 1));
}
void RunExampleScene() {
	int xStatic = 0, xDynamic = 1, xKinematic = 2;

	/// Floor
	Texture_ptr floorTexture = std::make_shared<Texture>();
	floorTexture->Load("C:/Users/omars/Documents/VS Projects/SlaviEngine/game-engine/Resources/awesomeFace.png");
	AddBoxActor(xStatic, std::make_shared<Transform>(glm::fvec3(0.0f, 0.0f, 0.0f), glm::fvec3(0.0f), glm::fvec3(10.0f, 1.0f, 10.0f)), glm::fvec3(0.2f, 0.4f, 0.6f), floorTexture);

	/// Light cube
	Texture_ptr lightTexture = std::make_shared<Texture>();
	lightTexture->Load("C:/Users/omars/Documents/VS Projects/SlaviEngine/game-engine/Resources/whiteTex.png");
	AddBoxActor(xStatic, std::make_shared<Transform>(glm::fvec3(12.0f, 12.0f, 12.0f), glm::fvec3(0.0f), glm::fvec3(1.0f)), glm::fvec3(1.0f, 1.0f, 1.0f), lightTexture, true);

	/// Platforms
	Texture_ptr platformTexture = std::make_shared<Texture>();
	platformTexture->Load("C:/Users/omars/Documents/VS Projects/SlaviEngine/game-engine/Resources/containerDiffuseMap.png");
	AddBoxActor(xKinematic, std::make_shared<Transform>(glm::fvec3(0.0f, 2.0f, 0.0f), glm::fvec3(0.0f), glm::fvec3(2.0f, 0.2f, 2.0f)), glm::fvec3(1.0f, 1.0f, 1.0f), platformTexture, false, glm::fvec3(0.0f, 2.0f, -3.0f), glm::fvec3(0.0f, 2.0f, 3.0f));
	AddBoxActor(xKinematic, std::make_shared<Transform>(glm::fvec3(-4.0f, 4.0f, 0.0f), glm::fvec3(0.0f), glm::fvec3(2.0f, 0.2f, 2.0f)), glm::fvec3(1.0f, 0.0f, 0.0f), platformTexture, false, glm::fvec3(-4.0f, 4.0f, 3.0f), glm::fvec3(-4.0f, 4.0f, -3.0f));
	AddBoxActor(xStatic, std::make_shared<Transform>(glm::fvec3(-8.0f, 6.0f, 0.0f), glm::fvec3(0.0f), glm::fvec3(2.0f, 0.2f, 2.0f)), glm::fvec3(0.0f, 1.0f, 0.0f), platformTexture, false, glm::fvec3(-6.0f, 7.0f, 2.0f), glm::fvec3(-10.0f, 5.0f, -2.0f));
	AddBoxActor(xKinematic, std::make_shared<Transform>(glm::fvec3(-8.0f, 8.0f, -4.0f), glm::fvec3(0.0f), glm::fvec3(2.0f, 0.2f, 2.0f)), glm::fvec3(0.8f, 0.5f, 0.0f), platformTexture, false, glm::fvec3(-8.0f, 11.0f, -4.0f), glm::fvec3(-8.0f, 5.0f, -4.0f));

	/// Random Boxes
	AddBoxActor(xDynamic, std::make_shared<Transform>(glm::fvec3(4.0f, 2.0f, 4.0f), glm::fvec3(0.0f), glm::fvec3(1.0f)), glm::fvec3(0.3f, 0.1f, 1.0f), platformTexture);

	//AddModelSingleMeshActor(bStatic, std::make_shared<Transform>(glm::fvec3(-8.0f, 8.0f, -4.0f), glm::fvec3(0.0f), glm::fvec3(2.0f, 2.0f, 2.0f)), "../cube.obj");

	//AddBoxActor(bDynamic, std::make_shared<Transform>(glm::fvec3(0.0f, 1.0f, -2.0f), glm::fvec3(0.0f), glm::fvec3(0.5f)),             glm::fvec3(1.0f, 1.0f, 0.0f));
	AddPlayer(std::make_shared<Transform>(glm::fvec3(2.0f, 8.0f, 2.0f), glm::fvec3(0.0f), glm::fvec3(0.3f, 0.3f, 0.3f)), glm::fvec3(0.0f, 1.0f, 1.0f), platformTexture);
}



ExampleScene::ExampleScene(RenderScene_ptr renderScene, PhysicsScene_ptr physicsScene) : Scene(renderScene, physicsScene) {
	mRenderScene->mRenderFunction = std::bind(&ExampleScene::Render, this, std::placeholders::_1);
	mPhysicsScene->mUpdateFunction = std::bind(&ExampleScene::UpdatePhysics, this);

	mRenderScene->mMainCamera = Camera::CreatePerspectiveCamera(45.0f, 0.1f, 1000.f);
	mRenderScene->mMainCamera->SetLocalTranslation(glm::fvec3(0.0f, 0.15f, 0.0f));

	ResourceManager::GetInstance().Init();
	ResourceManager::GetInstance().Mount("C:/DefinitelyNotWindows/Tracks/GameDev/Models");

	//mRenderScene->mMainCamera->SetEditorTranslation(glm::fvec3(0.0f, 5.0f, 20.0f));
	//mRenderScene->mMainCamera->SetEditorTranslation(glm::fvec3(0.0f, 30.0f, 3.0f));
	//mRenderScene->mMainCamera->SetEditorRotationDegrees(glm::fvec3(-30.0f, 0.0f, 0.0f));

	InitGraphcisPipeline();
}


RHIVertexShaderRef mVertexShaderRHINormal, mVertexShaderRHIModel;
RHIPixelShaderRef mPixelShaderRHINormal, mPixelShaderRHIModel;

RHIBoundShaderStateRef mShaderStateNormal, mShaderStateModel;

void ExampleScene::InitGraphcisPipeline(int shaderMode)
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
	std::vector<uint8_t> VertexShaderBytesModel, PixelShaderBytesModel;

	//std::copy(SimpleVertexShaderCode.begin(), SimpleVertexShaderCode.end(), std::back_inserter(VertexShaderBytes));
	//std::copy(SimplePixelShaderCode.begin(), SimplePixelShaderCode.end(), std::back_inserter(PixelShaderBytes));
	//std::copy(SimpleLightedVertexShaderCode.begin(), SimpleLightedVertexShaderCode.end(), std::back_inserter(VertexShaderBytes));
	//std::copy(SimpleLightedPixelShaderCode.begin(), SimpleLightedPixelShaderCode.end(), std::back_inserter(PixelShaderBytes));
	//std::copy(SimpleModelLoadingVS.begin(), SimpleModelLoadingVS.end(), std::back_inserter(VertexShaderBytesModel));
	//std::copy(SimpleModelLoadingFS.begin(), SimpleModelLoadingFS.end(), std::back_inserter(PixelShaderBytesModel));

	/*mVertexShaderRHINormal = OpenGLShaderCompiler::ManuallyCreateVertexShader(RHICmdList, VertexShaderBytes, 1);
	mPixelShaderRHINormal = OpenGLShaderCompiler::ManuallyCreatePixelShader(RHICmdList, PixelShaderBytes, 0);
	mVertexShaderRHIModel = OpenGLShaderCompiler::ManuallyCreateVertexShader(RHICmdList, VertexShaderBytesModel, 1);
	mPixelShaderRHIModel = OpenGLShaderCompiler::ManuallyCreatePixelShader(RHICmdList, PixelShaderBytesModel, 0);*/

	//switch (shaderMode)
	//{
	//case 0: // normal color
	//	std::copy(SimpleVertexShaderCode.begin(), SimpleVertexShaderCode.end(), std::back_inserter(VertexShaderBytes));
	//	std::copy(SimplePixelShaderCode.begin(), SimplePixelShaderCode.end(), std::back_inserter(PixelShaderBytes));
	//	mRenderScene->mVertexShaderRHI = OpenGLShaderCompiler::ManuallyCreateVertexShader(RHICmdList, VertexShaderBytes, 1);
	//	mRenderScene->mPixelShaderRHI = OpenGLShaderCompiler::ManuallyCreatePixelShader(RHICmdList, PixelShaderBytes, 0);
	//	break;
	//case 1: // texture 
	//	std::copy(SimpleModelLoadingVS.begin(), SimpleModelLoadingVS.end(), std::back_inserter(VertexShaderBytesModel));
	//	std::copy(SimpleModelLoadingFS.begin(), SimpleModelLoadingFS.end(), std::back_inserter(VertexShaderBytesModel));
	//	mRenderScene->mVertexShaderRHI = OpenGLShaderCompiler::ManuallyCreateVertexShader(RHICmdList, VertexShaderBytesModel, 1);
	//	mRenderScene->mPixelShaderRHI = OpenGLShaderCompiler::ManuallyCreatePixelShader(RHICmdList, PixelShaderBytesModel, 0);
	//	break;
	//default:
	//	break;
	//}

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

	//mRenderScene->mVertexShaderRHI = OpenGLShaderCompiler::ManuallyCreateVertexShader(RHICmdList, VertexShaderBytes, 2);
	//mRenderScene->mPixelShaderRHI = OpenGLShaderCompiler::ManuallyCreatePixelShader(RHICmdList, PixelShaderBytes, 0);
	//mShaderStateNormal = RHICmdList.CreateBoundShaderState(gStaticMeshVertexDeclaration.VertexDeclarationRHI, mRenderScene->mVertexShaderRHI, mRenderScene->mPixelShaderRHI);

	//mRenderScene->mVertexShaderRHI = OpenGLShaderCompiler::ManuallyCreateVertexShader(RHICmdList, VertexShaderBytesModel, 1);
	//mRenderScene->mPixelShaderRHI = OpenGLShaderCompiler::ManuallyCreatePixelShader(RHICmdList, PixelShaderBytesModel, 0);
	//mShaderStateModel = RHICmdList.CreateBoundShaderState(gStaticMeshVertexDeclaration.VertexDeclarationRHI, mRenderScene->mVertexShaderRHI, mRenderScene->mPixelShaderRHI);

	//bindShaderMode(RHICmdList, shaderMode);

	/*mRenderScene->mUniformBuffer = UniformBufferRef<PrimitiveUniformShaderParameters>::CreateUniformBufferImmediate(
		PrimitiveUniformShaderParameters(), EUniformBufferUsage::UniformBuffer_MultiFrame);

	gDynamicRHI->RHISetShaderUniformBuffer(mRenderScene->mVertexShaderRHI, 0, mRenderScene->mUniformBuffer);*/

	mShaderProgram = ShaderProgram(SimpleLightedVertexShaderCode, SimpleLightedPixelShaderCode);
	mShaderProgram.SetIntUniform("tex", 0);

	mShaderProgram2 = ShaderProgram(SimpleLightedVertexShaderCode, SimpleLighterPixelShaderCode);
}
void ExampleScene::bindShaderMode(RHICommandList& RHICmdList, int shaderMode) {
	//switch (shaderMode)
	//{
	//case 0: // normal color
	//	mRenderScene->mVertexShaderRHI = mVertexShaderRHINormal.GetReference();
	//	mRenderScene->mPixelShaderRHI = mPixelShaderRHINormal.GetReference();
	//	break;
	//case 1: // texture 
	//	mRenderScene->mVertexShaderRHI = mVertexShaderRHIModel.GetReference();
	//	mRenderScene->mPixelShaderRHI = mPixelShaderRHIModel.GetReference();
	//	break;
	//default:
	//	break;
	//}
	/*mRenderScene->mBoundShaderState = RHICmdList.CreateBoundShaderState(gStaticMeshVertexDeclaration.VertexDeclarationRHI, mRenderScene->mVertexShaderRHI, mRenderScene->mPixelShaderRHI);
	mRenderScene->mUniformBuffer = UniformBufferRef<PrimitiveUniformShaderParameters>::CreateUniformBufferImmediate(
		PrimitiveUniformShaderParameters(), EUniformBufferUsage::UniformBuffer_MultiFrame);

	gDynamicRHI->RHISetShaderUniformBuffer(mRenderScene->mVertexShaderRHI, 0, mRenderScene->mUniformBuffer);*/


	switch (shaderMode)
	{
	case 0: // normal color
		mRenderScene->mVertexShaderRHI = mVertexShaderRHINormal.GetReference();
		mRenderScene->mPixelShaderRHI = mPixelShaderRHINormal.GetReference();
		mRenderScene->mBoundShaderState = mShaderStateNormal.GetReference();
		break;
	case 1: // texture 
		mRenderScene->mVertexShaderRHI = mVertexShaderRHIModel.GetReference();
		mRenderScene->mPixelShaderRHI = mPixelShaderRHIModel.GetReference();
		mRenderScene->mBoundShaderState = mShaderStateModel.GetReference();
		break;
	default:
		break;
	}
}

void ExampleScene::UpdatePhysics() {
	double currentTime = glfwGetTime(); // really?
	mDeltaTime = static_cast<float>(currentTime - mLastTime);
	mLastTime = currentTime;

	std::vector<Actor_ptr> vec = SceneManager::GetInstance().GetActors();
	for (auto actor : vec) {
		if (!actor->kinematic) continue;
		
		PDynamicActor_ptr dynamicActor = std::static_pointer_cast<PDynamicActor>(actor->mPhysicsActor.lock());
		Transform_ptr oldTransform = actor->GetTransform();
		glm::fvec3 pos = oldTransform->GetTranslation();
		if (glm::distance(pos, actor->originalPos) > 3.0f) actor->dir = !actor->dir;
		glm::fvec3 dest = actor->dir ? actor->limit1 : actor->limit2;

		glm::fvec3 dir = dest - pos;
		glm::fvec3 newPos = pos + glm::normalize(dir) * 2.0f * (float)mDeltaTime;
		oldTransform->SetTranslation(newPos);
		dynamicActor->SetKinematicTarget(oldTransform);
	}

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
			//bindShaderMode(RHICmdList, actor->mShaderMode);
			RHICmdList.SetBoundShaderState(mRenderScene->mBoundShaderState);

			RenderActor_ptr renderActor = actor->mRenderActor.lock();
			if (!renderActor) continue;
			Transform_ptr transform = renderActor->mTransform;
			StaticMesh_ptr mesh = renderActor->mMesh;
			Texture_ptr texture = actor->GetTexture();
			texture->Bind();

			// set the vertex buffer to be our data source
			RHICmdList.SetStreamSource(0, mesh->VertexBuffer, 0);

			// make sure all matricies are calculated if any changes occurred
			transform->EnsureUpdated();

			// Update shader paramaters
			PrimitiveUniformShaderParameters Parameters;
			Parameters.LocalToWorld = ViewProjection * transform->GetLocalToWorld();
			Parameters.modelMatrix = transform->GetLocalToWorld();
			if (actor->lighter) {
				mShaderProgram2.Activate();
				mShaderProgram2.SetFloatMatrix4Uniform("mvp", Parameters.LocalToWorld);
				mShaderProgram2.SetFloatMatrix4Uniform("modelMat", Parameters.modelMatrix);
			} else {
				mShaderProgram.Activate();
				mShaderProgram.SetFloatMatrix4Uniform("mvp", Parameters.LocalToWorld);
				mShaderProgram.SetFloatMatrix4Uniform("modelMat", Parameters.modelMatrix);
			}
			//mRenderScene->mUniformBuffer.UpdateUniformBufferImmediate(Parameters);

			// this is yet fixed to draw triangles, in some cases or might be preffered
			// by the user, we may need to determine the topology
			// this will issue a draw call with index buffer
			RHICmdList.DrawIndexedPrimitive(mesh->IndexBuffer, 0, 0, mesh->NumVertices, 0, mesh->NumTriangles, 1);
		}
	}
	RHICmdList.EndRenderPass();
}