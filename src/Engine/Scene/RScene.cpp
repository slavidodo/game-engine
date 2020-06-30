
#include "pch.h"
#include "RScene.h"

void RScene::AddActor(RActor_ptr actor) {
	 mActors.push_back(actor);
}	 
void RScene::RemoveActor(RActor_ptr actor) {
	auto it = std::find(mActors.begin(), mActors.end(), actor);
	if (it != mActors.end()) mActors.erase(it);
}