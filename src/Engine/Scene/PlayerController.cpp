
#include "pch.h"
#include "PlayerController.h"

PlayerController::PlayerController(Actor_ptr player, Camera_ptr camera) : mPlayer(player), mCamera(camera) {}

void PlayerController::MovePlayer(Utils::Direction dir) {
	float movementSpeed = 0.01f;
	glm::fvec3 front = mCamera->mFront;
	glm::fvec3 right = glm::cross(front, glm::fvec3(0.0f, 1.0f, 0.0f));
	
	glm::fvec3 displacement = glm::fvec3(0.0f);
	switch (dir) {
	case Utils::Direction::Forward:
		displacement = front * movementSpeed;
		break;
	case Utils::Direction::Backward:
		displacement = -front * movementSpeed;
		break;
	case Utils::Direction::Right:
		displacement = right * movementSpeed;
		break;
	case Utils::Direction::Left:
		displacement = -right * movementSpeed;
		break;
	}
	displacement.y = 0.0f;

	mPlayer->Move(displacement);
}
void PlayerController::RotateCamera(double xDelta, double yDelta) {
	mCamera->Rotate(xDelta, yDelta);
}
void PlayerController::JumpPlayer() {
	if (mPlayer)
		mPlayer->Jump();
}