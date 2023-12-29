#pragma once
#include "stdafx.h"

#include "Utilities/Singleton.h"
#include "RenderingParameters.h"

class Window : public Singleton<Window>
{
	friend class Singleton<Window>;

protected:
	enum Codes { NOT_LOADED, SUCCESSFUL_LOAD, UNSUCCESSFUL_LOAD };

	glm::ivec2 _size;
	GLFWwindow* _window;
	uint8_t	_windowState;

protected:
	Window();

public:

	bool initialize(const std::string& title, const uint16_t width, const uint16_t height, const uint8_t openGL4Version = 6);

	glm::ivec2 getSize() { return _size; }

	void changedSize(const uint16_t width, const uint16_t height);

	void startRenderingCycle();
};
