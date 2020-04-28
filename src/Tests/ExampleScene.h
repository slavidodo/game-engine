
#ifndef TESTS_EXAMPLE_SCENE_H
#define TESTS_EXAMPLE_SCENE_H

#include "../Engine/Scene/Scene.h"
#include "../Engine/Core/StaticMesh.h"

class ExampleScene : public Scene
{
public:
	ExampleScene();

	void Render() override final;

private:
	std::vector<std::tuple<Transform_ptr, StaticMesh_ptr>> mToRenderElements;
};

#endif // TESTS_EXAMPLE_SCENE