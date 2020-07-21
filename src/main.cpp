
#include "pch.h"

#include "Engine/Platform/CoreApplication.h"
#include "Engine/Platform/Window.h"
#include "Engine/Filesystem/ResourceManager.h"
#include "Engine/Physics/PhysicsEngine.h"
#include "Engine/Scene/SceneManager.h"
#include "Tests/ExampleScene.h"
#include "Tests/PhysicsTest.h"

const std::string WINDOW_TITLE = "Game Engine";
constexpr int32_t WINDOW_WIDTH = 1024;
constexpr int32_t WINDOW_HEIGHT = 768;

#include "Engine/OpenGLRHI/OpenGLDynamicRHI.h"

#if defined(DIRECTX12_RENDERER_BACKEND)
#	include "Engine/D3D12RHI/D3D12DynamicRHI.h"
#	define RENDERING_BACKEND D3D12DynamicRHI
#elif defined(DIRECTX11_RENDERER_BACKEND)
#	include "Engine/D3D12RHI/D3D11DynamicRHI.h"
#	define RENDERING_BACKEND D3D11DynamicRHI
#elif defined(VULKAN_RENDERER_BACKEND)
#	include "Engine/VulkanRHI/VulkanDynamicRHI.h"
#	define RENDERING_BACKEND VulkanDynamicRHI
#else
#	define RENDERING_BACKEND OpenGLDynamicRHI
#	define RENDERING_OPENGL_FALLBACK
#endif

using Context = RENDERING_BACKEND;



int main(int argc, char* argv[])
{
	/* Physics tests (run without errors)*/
	//PhysicsTester pt;
	//pt.RunTests();

	std::vector<std::string> args(argv, argv + argc);

	// initialize core app
	if (!gApplication.Init())
		return 1;

	// initialize window
	if (!gWindow.Init(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT))
		return 1;

	// TODO the whole process of creating a window should involve creating its own context
	// resources and manage it on its Init/Terminate

	gDynamicRHI = new Context;

	if (!gDynamicRHI->Init()) {
		delete gDynamicRHI;

#ifndef RENDERING_OPENGL_FALLBACK
		gDynamicRHI = new OpenGLDynamicRHI();
		if (!gDynamicRHI->Init()) {
			delete gDynamicRHI;
			return 1;
		}
#else
		return 1;
#endif
	}

	// show window
	gWindow.Show();

	// TODO: should be done in a more semantic way
	// DynamicRHI has Init function, we could add PostInit
	// make sure to Update the viewport at least once
	// 
	gWindow.SetWindowSize(gWindow.GetWindowSize());

	// TODO: (this is hardcoded) Example scene (cube, camera)
	// TODO: The scene handles drawing but in real-life, it shouldn't
	// it should account for batching, ... and so the workflow of drawing
	// should be moved to a separate class



	// TODO: low-priority
	// we should have a default scene with a cube, camera and light
	// also, we could add premade scenes that contain some objects
	// and scripts as a tutorial for new users
	// in order to do that, we need to firstly support serializing
	// and agreed-on structure of how a scene could be stored
	// in order to make this work, the engine may ship with extra
	// objects, ...
	PhysicsSceneDescriptor desc;
	desc.SetGravityForce(glm::fvec3(0.0f, -9.8f, 0.0f));


	SceneManager::GetInstance().SetCurrentScene(std::make_shared<ExampleScene>(std::make_shared<RenderScene>(), PhysicsScene::CreateScene(desc)));
	RunExampleScene();


	// run application's main loop
	gApplication.RunMainLoop();

	SceneManager::GetInstance().SetCurrentScene(nullptr);
	// shut down, remove context and related resources
	gDynamicRHI->Terminate();
	delete gDynamicRHI;

	// terminate window and core app
	gWindow.Terminate();
	gApplication.Terminate();

	return 0;
}