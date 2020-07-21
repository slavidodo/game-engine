
#include "pch.h"
#include "RenderScene.h"

void RenderScene::AddActor(RenderActor_ptr actor) {
	 mActors.push_back(actor);
}	 
void RenderScene::RemoveActor(RenderActor_ptr actor) {
	auto it = std::find(mActors.begin(), mActors.end(), actor);
	if (it != mActors.end()) mActors.erase(it);
}