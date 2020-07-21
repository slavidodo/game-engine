
#ifndef TESTS_EXAMPLE_SCENE_H
#define TESTS_EXAMPLE_SCENE_H

#include "../Engine/Scene/Scene.h"
#include "../Engine/Core/StaticMesh.h"

#include "../Engine/Scene/SceneManager.h"
#include "../Engine/Scene/Actor.h"

void AddCubeActor(bool bStatic, Transform_ptr transform);
void RunExampleScene();

class ExampleScene : public Scene
{
public:
	ExampleScene(RenderScene_ptr renderScene, PhysicsScene_ptr physicsScene);
	
	void UpdatePhysics();
	void Render(RHICommandList& RHICmdList);

private:
	/**
	 * Initialize materials, shaders, render passes
	 */
	void InitGraphcisPipeline();
	void RenderSceneActors(RHICommandList& RHICmdList);
};

#endif // TESTS_EXAMPLE_SCENE