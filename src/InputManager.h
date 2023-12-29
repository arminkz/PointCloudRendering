#pragma once
#include "stdafx.h"

#include "utilities/Singleton.h"


class InputManager : public Singleton<InputManager>
{
	friend class Singleton<InputManager>;

protected:
	enum MovementTypes : uint8_t
	{
		BOOM, CRANE, DOLLY, ORBIT_XZ, ORBIT_Y, PAN, TILT, TRUCK, ZOOM, NUM_MOVEMENT_TYPES
	};

	enum EventType : uint8_t
	{
		KEY_PRESS, KEY_RELEASE, MOUSE_CURSOR, MOUSE_BUTTON, MOUSE_SCROLL, RESIZE
	};

protected:
	// [Movements constraints]
	static const std::vector<float> MOVEMENT_SPEED;						//!< Movement speed for each type of movement

	static const glm::vec2			CURSOR_POS;							//!< Default cursor position, just to be ignored
	static const float				MOVEMENT_MULTIPLIER;				//!< Multiplier for default dolly speed after each movement
	static const float				MOVEMENT_SPEED_UP;					//!< Multiplicator for general dolly speed as another key is pressed

	// [Keys constraints]
	static const int				BOOM_KEY;
	static const int				CRANE_KEY;
	static const int				DOLLY_POS_KEY;
	static const int				DOLLY_NEG_KEY;
	static const int				DOLLY_SPEED_UP_KEY;
	static const int				ITERATE_LIDAR;
	static const int				ORBIT_XZ_KEY;
	static const int				ORBIT_Y_KEY;
	static const int				PAN_KEY;
	static const int				RESET_KEY;
	static const int				SCREENSHOT_KEY;
	static const int				TILT_KEY;
	static const int				TRUCK_POS_KEY;
	static const int				TRUCK_NEG_KEY;
	static const int REAGGRO_KEY;

protected:
	GLFWwindow* _window;
	glm::vec2 _lastCursorPosition;
	bool _leftClickPressed, _rightClickPressed;
	

protected:

	InputManager();

	static std::vector<float> getMovementSpeed();

	// [Higher Level Callbacks]
	void processPressedKeyEvent(const int key, const int mods);
	void processReleasedKeyEvent(const int key, const int mods);
	void processMouseDrag(const int xOffset, const int yOffset);

public:

	void initialize(GLFWwindow* window);

	// [Callbacks (GLFW)]
	static void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
	static void mouseCursorEvent(GLFWwindow* window, double xpos, double ypos);
	static void mouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset);
	static void resizeEvent(GLFWwindow* window, int width, int height);
	static void windowRefresh(GLFWwindow* window);

	// [Getter & Setter]
};

