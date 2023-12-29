#include "InputManager.h"

//#include "geometry/BasicOperations.h"
//#include "core/SSAOScene.h"
//#include "core/Renderer.h"
//#include "core/camera/Camera.h"

#include "GUI.h"
#include "Window.h"
#include "Renderer.h"

// [Static members initialization]

const std::vector<float> InputManager::MOVEMENT_SPEED = getMovementSpeed();

const glm::vec2	InputManager::CURSOR_POS = glm::vec2(-1.0f, -1.0f);					//!< Default cursor position, just to be ignored
const float InputManager::MOVEMENT_MULTIPLIER = 0.1f;
const float InputManager::MOVEMENT_SPEED_UP = 1.0f;

const int InputManager::BOOM_KEY = GLFW_KEY_UP;
const int InputManager::CRANE_KEY = GLFW_KEY_DOWN;
const int InputManager::TRUCK_POS_KEY = GLFW_KEY_RIGHT;
const int InputManager::TRUCK_NEG_KEY = GLFW_KEY_LEFT;

const int InputManager::DOLLY_POS_KEY = GLFW_KEY_W;
const int InputManager::DOLLY_NEG_KEY = GLFW_KEY_S;

const int InputManager::ORBIT_XZ_KEY = GLFW_KEY_Y;
const int InputManager::ORBIT_Y_KEY = GLFW_KEY_X;

const int InputManager::REAGGRO_KEY = GLFW_KEY_B;

/*
const int InputManager::DOLLY_SPEED_UP_KEY = GLFW_MOD_SHIFT;
const int InputManager::ITERATE_LIDAR = GLFW_KEY_I;
const int InputManager::PAN_KEY = GLFW_KEY_P;
const int InputManager::RESET_KEY = GLFW_KEY_R;
const int InputManager::SCREENSHOT_KEY = GLFW_KEY_K;
const int InputManager::TILT_KEY = GLFW_KEY_T;
*/


/// [Protected methods]

InputManager::InputManager() :
	_lastCursorPosition(CURSOR_POS), _leftClickPressed(false), _rightClickPressed(false), _window(nullptr)
{
}

std::vector<float> InputManager::getMovementSpeed()
{
	std::vector<float> movSpeed(NUM_MOVEMENT_TYPES);

	movSpeed[BOOM] = 0.3f;
	movSpeed[CRANE] = 0.3f;
	movSpeed[DOLLY] = 0.3f;
	movSpeed[TRUCK] = 0.3f;
	movSpeed[ORBIT_XZ] = 0.05f;
	movSpeed[ORBIT_Y] = 0.03f;
	movSpeed[PAN] = 0.002f;
	movSpeed[TILT] = 0.002f;
	movSpeed[ZOOM] = 0.008f;

	return movSpeed;
}

void InputManager::processPressedKeyEvent(const int key, const int mods)
{
	Camera* camera = Renderer::getInstance()->getCamera();

	switch (key)
	{
	case InputManager::ORBIT_XZ_KEY:
		if (mods == GLFW_MOD_CONTROL)
		{
			camera->orbitXZ(-MOVEMENT_SPEED[ORBIT_XZ]);
		}
		else
		{
			camera->orbitXZ(MOVEMENT_SPEED[ORBIT_XZ]);
		}
		break;

	case InputManager::ORBIT_Y_KEY:
		if (mods == GLFW_MOD_CONTROL)
		{
			camera->orbitY(-MOVEMENT_SPEED[ORBIT_Y]);
		}
		else
		{
			camera->orbitY(MOVEMENT_SPEED[ORBIT_Y]);
		}
		break;

	case InputManager::DOLLY_POS_KEY:
		camera->dolly(MOVEMENT_SPEED[DOLLY]);
		break;

	case InputManager::DOLLY_NEG_KEY:
		camera->dolly(-MOVEMENT_SPEED[DOLLY]);
		break;

	case InputManager::TRUCK_POS_KEY:
		camera->truck(MOVEMENT_SPEED[TRUCK]);
		break;

	case InputManager::TRUCK_NEG_KEY:
		camera->truck(-MOVEMENT_SPEED[TRUCK]);
		break;

	case InputManager::BOOM_KEY:
		camera->boom(MOVEMENT_SPEED[BOOM]);
		break;

	case InputManager::CRANE_KEY:
		camera->crane(MOVEMENT_SPEED[BOOM]);
		break;

	case InputManager::REAGGRO_KEY:
		Renderer::getInstance()->getCurrentScene()->initialize();
		break;

	};
}

void InputManager::processReleasedKeyEvent(const int key, const int mods)
{
}

void InputManager::processMouseDrag(const int xOffset, const int yOffset)
{
	Camera* camera = Renderer::getInstance()->getCamera();

	camera->pan(-MOVEMENT_SPEED[PAN] * xOffset);
	camera->tilt(-MOVEMENT_SPEED[TILT] * yOffset);
}


/// [Public methods]

void InputManager::initialize(GLFWwindow* window)
{
	_window = window;
}

void InputManager::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputManager* inputManager = InputManager::getInstance();

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		inputManager->processPressedKeyEvent(key, mods);
	}
	else
	{
		inputManager->processReleasedKeyEvent(key, mods);
	}
}

void InputManager::mouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	if (GUI::getInstance()->isMouseActive()) return;

	InputManager* inputManager = InputManager::getInstance();
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		inputManager->_leftClickPressed = action == GLFW_PRESS;
		inputManager->_lastCursorPosition = CURSOR_POS;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		inputManager->_rightClickPressed = action == GLFW_PRESS;
		inputManager->_lastCursorPosition = CURSOR_POS;
	}
}

void InputManager::mouseCursorEvent(GLFWwindow* window, double xpos, double ypos)
{
	if (GUI::getInstance()->isMouseActive()) return;
	
	InputManager* inputManager = InputManager::getInstance();
	inputManager->_leftClickPressed &= glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	inputManager->_rightClickPressed &= glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

	if (inputManager->_leftClickPressed || inputManager->_rightClickPressed)
	{
		if (inputManager->_lastCursorPosition.x >= 0.0f) {
			if (xpos != inputManager->_lastCursorPosition.x || ypos != inputManager->_lastCursorPosition.y)
			{
				inputManager->processMouseDrag(xpos - inputManager->_lastCursorPosition.x, ypos - inputManager->_lastCursorPosition.y);
			}
		}
	}
	inputManager->_lastCursorPosition = glm::vec2(xpos, ypos);
}

void InputManager::mouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{
	if (GUI::getInstance()->isMouseActive()) return;

	Camera* camera = Renderer::getInstance()->getCamera();
	camera->zoom(yoffset);
}

void InputManager::resizeEvent(GLFWwindow* window, int width, int height)
{
	if (width == 0 || height == 0) return;

	Window::getInstance()->changedSize(width, height);
	Renderer::getInstance()->sizeChanged(width, height);
}

void InputManager::windowRefresh(GLFWwindow* window)
{
	//spdlog::info("window refresh event");
}
