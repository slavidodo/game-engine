
#include <framework/pch.h>

#include "scene.h"

using namespace framework::graphics;

void Scene::render()
{
	if (!m_mainCamera)
		return;

	// determine the frustum of the camera and get all active scene objects
}