
#include "pch.h"

#include "InputManager.h"

void InputManager::OnKeyCallback(int32_t key, int32_t scancode, int32_t action, int32_t mods) {
	/*switch (key) {
	case GLFW_KEY_W:
		SceneManager::GetInstance().MovePlayer(SceneManager::MovementDirection::Forward);
		break;
	case GLFW_KEY_S:
		SceneManager::GetInstance().MovePlayer(SceneManager::MovementDirection::Backward);
		break;
	case GLFW_KEY_A:
		SceneManager::GetInstance().MovePlayer(SceneManager::MovementDirection::Left);
		break;
	case GLFW_KEY_D:
		SceneManager::GetInstance().MovePlayer(SceneManager::MovementDirection::Right);
		break;
	case GLFW_KEY_Q:
		SceneManager::GetInstance().MovePlayer(SceneManager::MovementDirection::Downward);
		break;
	case GLFW_KEY_E:
		SceneManager::GetInstance().MovePlayer(SceneManager::MovementDirection::Upward);
		break;
	case GLFW_KEY_SPACE:
		SceneManager::GetInstance().JumpPlayer();
		break;
	}*/
}
void InputManager::OnMouseCallback(double xPos, double yPos) {
	if (bFirstFrame) {
		bFirstFrame = false;
		return;
	}
	double xDelta = xPos - mMousePos.first;
	double yDelta = yPos - mMousePos.second;
	SceneManager::GetInstance().RotatePlayer(xDelta, yDelta);
	mMousePos = {xPos, yPos};
}

void InputManager::CheckInputs() {
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_W))
		SceneManager::GetInstance().MovePlayer(SceneManager::MovementDirection::Forward);
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_S))
		SceneManager::GetInstance().MovePlayer(SceneManager::MovementDirection::Backward);
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_A))
		SceneManager::GetInstance().MovePlayer(SceneManager::MovementDirection::Left);
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_D))
		SceneManager::GetInstance().MovePlayer(SceneManager::MovementDirection::Right);
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_E))
		SceneManager::GetInstance().MovePlayer(SceneManager::MovementDirection::Upward);
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_Q))
		SceneManager::GetInstance().MovePlayer(SceneManager::MovementDirection::Downward);
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_SPACE))
		SceneManager::GetInstance().JumpPlayer();
}