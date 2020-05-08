
#include "pch.h"

#include "SceneManager.h"

void SceneManager::Render(RHICommandList& RHICmdList)
{
	if (mCurrentScene) {
		mCurrentScene->Render(RHICmdList);
	}
}