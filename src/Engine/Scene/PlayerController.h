#pragma once

#include "Actor.h"
#include "../Core/Camera.h"
#include "../Utils.h"

class PlayerController;
typedef std::shared_ptr<PlayerController> PlayerController_ptr;

class PlayerController {
public:
	PlayerController(Actor_ptr player, Camera_ptr camera);

	void MovePlayer(Utils::Direction dir);
	void RotateCamera(double xDelta, double yDelta);
	void JumpPlayer();


protected:
	Actor_ptr mPlayer;
	Camera_ptr mCamera;
};