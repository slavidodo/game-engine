
#include "pch.h"

#include "scene_manager.h"

using namespace framework::managers;

SceneManager g_sceneManager;

void SceneManager::render()
{
	if (m_currentScene) {
		m_currentScene->render();
	}
}