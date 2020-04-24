
#include "pch.h"

#include "SceneManager.h"

void SceneManager::Render()
{
	if (m_currentScene) {
		m_currentScene->Render();
	}
}