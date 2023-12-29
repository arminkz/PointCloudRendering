#pragma once
#include "stdafx.h"

#include "interface/fonts/font_awesome.hpp"
#include "interface/fonts/lato.hpp"
#include "interface/fonts/IconsFontAwesome5.h"
#include "Utilities/Singleton.h"
#include "RenderingParameters.h"


class GUI : public Singleton<GUI>
{
	friend class Singleton<GUI>;

protected:
	// GUI state
	bool _showAboutUs;						//!< About us window
	bool _showControls;						//!< Shows application controls
	bool _showFileDialog;					//!< Shows a file dialog that allows opening a point cloud in .ply format
	bool _showRenderingSettings;			//!< Displays a window which allows the user to modify the rendering parameters

protected:
	GUI();

	void createElements();

	void showAboutUsWindow();

	void showControls();

	void showFileDialog();

	void showRenderingSettings();

	void loadFonts();

	void loadImGUIStyle();

	static void leaveSpace(const unsigned numSlots);

	static void renderHelpMarker(const char* message);


public:
	virtual ~GUI();

	void initialize(GLFWwindow* window, const int openglMinorVersion);

	void render();

	bool isMouseActive() { return ImGui::GetIO().WantCaptureMouse; }
};