
#include "pch.h"

#include "InputManager.h"

void InputManager::OnKeyCallback(int32_t key, int32_t scancode, int32_t action, int32_t mods) {
	/*switch (key) {
	case GLFW_KEY_W:
		SceneManager::GetInstance().MovePlayer(Camera::MovementDirection::Forward);
		break;
	case GLFW_KEY_S:
		SceneManager::GetInstance().MovePlayer(Camera::MovementDirection::Backward);
		break;
	case GLFW_KEY_A:
		SceneManager::GetInstance().MovePlayer(Camera::MovementDirection::Left);
		break;
	case GLFW_KEY_D:
		SceneManager::GetInstance().MovePlayer(Camera::MovementDirection::Right);
		break;
	case GLFW_KEY_Q:
		SceneManager::GetInstance().MovePlayer(Camera::MovementDirection::Downward);
		break;
	case GLFW_KEY_E:
		SceneManager::GetInstance().MovePlayer(Camera::MovementDirection::Upward);
		break;
	case GLFW_KEY_SPACE:
		SceneManager::GetInstance().JumpPlayer();
		break;
	}*/
}
void InputManager::OnMouseCallback(double xPos, double yPos) {
	if (bFirstFrame) {
		bFirstFrame = false;
	}
	else {
		double xDelta = xPos - mMousePos.first;
		double yDelta = yPos - mMousePos.second;
		SceneManager::GetInstance().RotateCamera(xDelta, yDelta);
	}
	mMousePos = {xPos, yPos};
}

void InputManager::CheckInputs() {
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_W))
		SceneManager::GetInstance().MovePlayer(Utils::Direction::Forward);
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_S))
		SceneManager::GetInstance().MovePlayer(Utils::Direction::Backward);
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_A))
		SceneManager::GetInstance().MovePlayer(Utils::Direction::Left);
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_D))
		SceneManager::GetInstance().MovePlayer(Utils::Direction::Right);
	/*if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_E))
		SceneManager::GetInstance().MovePlayer(Utils::Direction::Upward);
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_Q))
		SceneManager::GetInstance().MovePlayer(Utils::Direction::Downward);*/
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_SPACE))
		SceneManager::GetInstance().JumpPlayer();
	if (glfwGetKey(gWindow.GetGlfwWindow(), GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(gWindow.GetGlfwWindow(), true);
}