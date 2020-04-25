
#include "pch.h"

#include "SceneManager.h"

void SceneManager::Render()
{
	if (mCurrentScene) {
		mCurrentScene->Render();
	}
}