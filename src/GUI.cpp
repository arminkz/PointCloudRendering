#include "GUI.h"

#include "ImGuiFileDialog.h"
#include "Renderer.h"

/// [Protected methods]

GUI::GUI() :
	_showRenderingSettings(false), 
	_showAboutUs(false),
	_showControls(false), 
	_showFileDialog(false)
{
}

void GUI::createElements()
{
	ImGuiIO& io = ImGui::GetIO();

	if (_showRenderingSettings)		showRenderingSettings();
	if (_showAboutUs)				showAboutUsWindow();
	if (_showControls)				showControls();
	if (_showFileDialog)			showFileDialog();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(ICON_FA_COG "Settings"))
		{
			ImGui::MenuItem(ICON_FA_CUBE "Rendering", NULL, &_showRenderingSettings);
			ImGui::MenuItem(ICON_FA_SAVE "Open Point Cloud", NULL, &_showFileDialog);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(ICON_FA_QUESTION_CIRCLE "Help"))
		{
			ImGui::MenuItem(ICON_FA_INFO "About the project", NULL, &_showAboutUs);
			ImGui::MenuItem(ICON_FA_GAMEPAD "Controls", NULL, &_showControls);
			ImGui::EndMenu();
		}

		ImGui::SameLine();
		ImGui::SetCursorPosX(io.DisplaySize.x - 125);
		this->renderHelpMarker("Help text here");

		ImGui::SameLine(0, 20);
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::EndMainMenuBar();
	}
}

void GUI::leaveSpace(const unsigned numSlots)
{
	for (int i = 0; i < numSlots; ++i)
	{
		ImGui::Spacing();
	}
}

void GUI::renderHelpMarker(const char* message)
{
	ImGui::TextDisabled(ICON_FA_QUESTION);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(message);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void GUI::showAboutUsWindow()
{
	if (ImGui::Begin("About the project", &_showAboutUs))
	{
		ImGui::Text("Created by: Armin Kazemi    github.com/arminkz");
	}
	ImGui::End();
}

void GUI::showControls()
{
	if (ImGui::Begin("Scene controls", &_showControls))
	{
		ImGui::Columns(2, "ControlColumns"); // 4-ways, with border
		ImGui::Separator();
		ImGui::Text("Movement"); ImGui::NextColumn();
		ImGui::Text("Control"); ImGui::NextColumn();
		ImGui::Separator();

		const int NUM_MOVEMENTS = 11;
		const char* movement[] = { "Orbit (XZ)", "Undo Orbit (XZ)", "Orbit (Y)", "Undo Orbit (Y)", "Z Movement", "XY Movement", "Reset Camera", "Take Screenshot", "Zoom +/-",     "Pan",                                  "Tilt" };
		const char* controls[] = { "X",          "Ctrl + X",        "Y",         "Ctrl + Y",       "W, S",       "Arrow Keys",  "R",            "K",               "Scroll wheel", "Drag mouse horizontally", "Drag mouse vertically" };

		for (int i = 0; i < NUM_MOVEMENTS; i++)
		{
			ImGui::Text(movement[i]); ImGui::NextColumn();
			ImGui::Text(controls[i]); ImGui::NextColumn();
		}

		ImGui::Columns(1);
		ImGui::Separator();

	}
	ImGui::End();
}

void GUI::showFileDialog()
{
	ImGuiFileDialog::Instance()->OpenDialog("Choose Point Cloud", "Choose File", ".ply", ".");

	// display
	if (ImGuiFileDialog::Instance()->Display("Choose Point Cloud"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			Renderer::getInstance()->createSceneFromFile(filePathName);
		}

		// close
		ImGuiFileDialog::Instance()->Close();
		_showFileDialog = false;
	}
}

void GUI::showRenderingSettings()
{
	RenderingParameters* _renderingParams = Renderer::getInstance()->getRenderingParameters();
	if (ImGui::Begin("Rendering Settings", &_showRenderingSettings))
	{
		//ImGui::ColorEdit3("Background color", &_renderingParams->_backgroundColor[0]);

		this->leaveSpace(3);
		ImGui::Separator();
		ImGui::Text(ICON_FA_TREE "Point Cloud");

		this->leaveSpace(2);
		const char* visualizationTitles[] = { "Depth", "Colors", "Normals", "Full" };
		ImGui::NewLine();
		ImGui::SameLine(30, 0);
		ImGui::PushItemWidth(200.0f);
		ImGui::Combo("Visualization", &_renderingParams->_displayMode, visualizationTitles, IM_ARRAYSIZE(visualizationTitles));

		this->leaveSpace(1);
		ImGui::NewLine();
		ImGui::SameLine(30, 0);
		ImGui::PushItemWidth(200.0f);
		ImGui::SliderFloat("Point Size", &_renderingParams->_pointSize, 0.01f, 0.5f);


		this->leaveSpace(3);
		ImGui::Separator();
		ImGui::Text(ICON_FA_LIGHTBULB "Lighting");

		this->leaveSpace(2);
		ImGui::NewLine();
		ImGui::SameLine(30, 0);
		ImGui::PushItemWidth(200.0f);
		ImGui::ColorEdit3("Light Color", &_renderingParams->_lightColor[0]);

		this->leaveSpace(1);
		ImGui::NewLine();
		ImGui::SameLine(30, 0);
		ImGui::PushItemWidth(200.0f);
		ImGui::SliderFloat("Ambient Strength", &_renderingParams->_ambient_strength, 0.01f, 1.0f);

		this->leaveSpace(1);
		ImGui::NewLine();
		ImGui::SameLine(30, 0);
		ImGui::PushItemWidth(200.0f);
		ImGui::SliderFloat("Specular Strength", &_renderingParams->_specular_strength, 0.01f, 1.0f);


		this->leaveSpace(3);
		ImGui::Separator();
		ImGui::Text(ICON_FA_CUBE "Level of Detail");

		this->leaveSpace(2);
		ImGui::NewLine();
		ImGui::SameLine(30, 0);
		ImGui::PushItemWidth(200.0f);
		ImGui::Checkbox("Enable LoD and CPU Frustum Culling", &_renderingParams->_cpuCulling);
		this->leaveSpace(1);
		ImGui::NewLine();
		ImGui::SameLine(30, 0);
		ImGui::PushItemWidth(200.0f);
		ImGui::Text("Press B to recalculate.");
		//ImGui::Checkbox("Recalculate Every Frame", &_renderingParams->_cpuCullingEveryFrame);
	}
	ImGui::End();
}

GUI::~GUI()
{
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

/// [Public methods]

void GUI::initialize(GLFWwindow* window, const int openglMinorVersion)
{
	const std::string openGLVersion = "#version 4" + std::to_string(openglMinorVersion) + "0 core";

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();

	this->loadImGUIStyle();
	this->loadFonts();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(openGLVersion.c_str());
}

void GUI::render()
{
	bool show_demo_window = true;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	this->createElements();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::loadImGUIStyle()
{
	ImGui::StyleColorsDark();
}

void GUI::loadFonts()
{
	ImFontConfig cfg;
	ImGuiIO& io = ImGui::GetIO();

	std::copy_n("Lato", 5, cfg.Name);
	io.Fonts->AddFontFromMemoryCompressedBase85TTF(lato_compressed_data_base85, 15.0f, &cfg);

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	cfg.MergeMode = true;
	cfg.PixelSnapH = true;
	cfg.GlyphMinAdvanceX = 20.0f;
	cfg.GlyphMaxAdvanceX = 20.0f;
	std::copy_n("FontAwesome", 12, cfg.Name);

	io.Fonts->AddFontFromFileTTF("fonts/fa-regular-400.ttf", 13.0f, &cfg, icons_ranges);
	io.Fonts->AddFontFromFileTTF("fonts/fa-solid-900.ttf", 13.0f, &cfg, icons_ranges);
}