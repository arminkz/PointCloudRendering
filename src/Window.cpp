#include "Window.h"

#include "stdafx.h"

#include "GUI.h"
#include "Renderer.h"
#include "InputManager.h"
#include "RenderingParameters.h"


/// [Protected methods]

Window::Window() : Singleton(), _size(glm::ivec2(1,1)), _window(nullptr), _windowState(NOT_LOADED)
{
}

/// [Public methods]

void Window::changedSize(const uint16_t width, const uint16_t height)
{
	_size = glm::ivec2(width, height);
}

bool Window::initialize(const std::string& title, const uint16_t width, const uint16_t height, const uint8_t openGL4Version)
{
	_size = glm::ivec2(width, height);

	if (glfwInit() != GLFW_TRUE)
	{
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);										// Antialiasing
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);			// OpenGL Core Profile 4.5
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glm::clamp((int)openGL4Version, 1, 6));

	_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (_window == nullptr) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		spdlog::error("Error initializing GLEW.");
		return false;
	}

	spdlog::info("----------------------------------------");
	spdlog::info("GPU : {} ", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	spdlog::info("Vendor : {} ", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	spdlog::info("Version : {} ", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	spdlog::info("Shading Language Version : {} ", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	spdlog::info("----------------------------------------");

	// Callbacks to main events
	glfwSetWindowRefreshCallback(_window, InputManager::windowRefresh);
	glfwSetFramebufferSizeCallback(_window, InputManager::resizeEvent);
	glfwSetKeyCallback(_window, InputManager::keyEvent);
	glfwSetMouseButtonCallback(_window, InputManager::mouseButtonEvent);
	glfwSetCursorPosCallback(_window, InputManager::mouseCursorEvent);
	glfwSetScrollCallback(_window, InputManager::mouseScrollEvent);

	spdlog::info("Input callbacks are set.");

	// OpenGL environment
	Renderer::getInstance()->initialize();
	GUI::getInstance()->initialize(_window, openGL4Version);

	//InputManager::getInstance()->initialize(_window);

	_windowState = SUCCESSFUL_LOAD;

	//Load example point cloud to save time (should delete this later)
	//std::string examplePC = "C:\\Users\\Armin\\Downloads\\point_cloud.ply";
	//Renderer::getInstance()->createSceneFromFile(examplePC);

	return true;
}

void Window::startRenderingCycle()
{
	if (_windowState != SUCCESSFUL_LOAD) return;

	//InputManager* inputManager = InputManager::getInstance();

	while (!glfwWindowShouldClose(_window)) {
		//InputManager::getInstance()->windowRefresh(_window);

		Renderer::getInstance()->render();
		GUI::getInstance()->render();

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	spdlog::info("Terminating GLFW window...");
	glfwDestroyWindow(_window);
	glfwTerminate();
}
