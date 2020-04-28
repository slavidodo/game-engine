
#include "pch.h"

#include "ExampleScene.h"

#include "../Engine/StaticMeshGenerator.h"
#include "../Engine/Platform/Window.h"

ExampleScene::ExampleScene()
{
	// todo; add a cube, a camera with a texture
	auto transform = Transform_ptr(new Transform(glm::fvec3(0), glm::fvec3(0), glm::fvec3(1)));
	auto cubeMesh = StaticMeshGenerator::CreateCube(0.5f);

	mToRenderElements.push_back({ std::move(transform), std::move(cubeMesh) });
}

void ExampleScene::Render()
{
	auto DynamicRHI = g_window.GetContext();
	for (auto&& t : mToRenderElements) {

	}
}