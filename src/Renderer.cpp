#include "Renderer.h"

#include "stdafx.h"

#include "Window.h"

/// [Protected methods]

Renderer::Renderer() :
	_scene(nullptr),
	_renderingParams(std::make_unique<RenderingParameters>())
{}

void Renderer::setupShaders()
{
	// Setup/Compile shaders
	_shader_depth = std::make_unique<Shader>("shaders/splat.vert", "shaders/splat_depth.frag");

	_shader_attr = std::make_unique<Shader>("shaders/splat.vert", "shaders/splat_attributes.frag");
	_shader_attr->use();
	_shader_attr->setInt("g_depth", 0);

	_shader_render_depth = std::make_unique<Shader>("shaders/render.vert", "shaders/render_depth.frag");
	_shader_render_depth->use();
	_shader_render_depth->setInt("g_depths", 0);

	_shader_render_colors = std::make_unique<Shader>("shaders/render.vert", "shaders/render_colors.frag");
	_shader_render_colors->use();
	_shader_render_colors->setInt("g_colors", 0);

	_shader_render_normals = std::make_unique<Shader>("shaders/render.vert", "shaders/render_normals.frag");
	_shader_render_normals->use();
	_shader_render_normals->setInt("g_normals", 0);

	_shader_render_full = std::make_unique<Shader>("shaders/render.vert", "shaders/render.frag");
	_shader_render_full->use();
	_shader_render_full->setInt("g_normals", 0);
	_shader_render_full->setInt("g_colors", 1);
}

void Renderer::setupGBuffers()
{
	//TODO: This function is needed to recalled if window size changes
	// Window Height/Width
	glm::ivec2 windowSize = Window::getInstance()->getSize();

	// Setup G-Buffers
	//glDeleteFramebuffers(1, &_gFrameBuffer);
	//
	glGenFramebuffers(1, &_gFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _gFrameBuffer);

	// depth buffer
	//glDeleteTextures(1, &_gDepthTex);
	//
	glGenTextures(1, &_gDepthTex);
	glBindTexture(GL_TEXTURE_2D, _gDepthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, windowSize[0], windowSize[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _gDepthTex, 0);

	// normal buffer
	//glDeleteTextures(1, &_gNormalTex);
	//
	glGenTextures(1, &_gNormalTex);
	glBindTexture(GL_TEXTURE_2D, _gNormalTex);
	// using RGBA because RGB doesn't work on my laptop, A is not actually used for anything
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, windowSize[0], windowSize[1], 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _gNormalTex, 0);

	// color/weight buffer (colorR, colorG, colorB, weight)
	//glDeleteTextures(1, &_gColorTex);
	//
	glGenTextures(1, &_gColorTex);
	glBindTexture(GL_TEXTURE_2D, _gColorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, windowSize[0], windowSize[1], 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _gColorTex, 0);

	unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		spdlog::error("Framebuffer not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Renderer::setupRenderQuad()
{
	float verts[] = {
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f };

	GLuint VBO;
	glGenBuffers(1, &VBO);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VAO;
}


/// [Public methods]

Renderer::~Renderer()
{
}

void Renderer::initialize()
{
	//initialize opengl
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE); // pure additive blend, no alpha multiplication

	glClearColor(_renderingParams->_backgroundColor.x, _renderingParams->_backgroundColor.y, _renderingParams->_backgroundColor.z, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//initialize/compile shaders
	setupShaders();

	setupGBuffers();
	_quadVAO = setupRenderQuad();

	//Create Camera
	glm::ivec2 windowSize = Window::getInstance()->getSize();
	_camera = std::make_unique<Camera>(windowSize[0], windowSize[1]);
}

void Renderer::render()
{
	//Clean screen
	glClearColor(_renderingParams->_backgroundColor.x, _renderingParams->_backgroundColor.y, _renderingParams->_backgroundColor.z, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Return if there is no active scene
	if (_scene == nullptr) return;

	glm::mat4 view = _camera->getViewMatrix();
	glm::mat4 proj = _camera->getProjectionMatrix();

	float epsilon = _renderingParams->_epsilonMultiplier * _renderingParams->_pointSize;

	glBindFramebuffer(GL_FRAMEBUFFER, _gFrameBuffer);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	// Clean to white for texture generation
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Depth pass
	_shader_depth->use();
	_shader_depth->setMat4("view_proj_matrix", proj * view);
	_shader_depth->setFloat("fov_y", _camera->getFovY());
	_shader_depth->setVec2("viewport_size", glm::vec2(_camera->getWidth(), _camera->getHeight()));
	_shader_depth->setFloat("epsilon", epsilon);
	_shader_depth->setFloat("particle_radius", _renderingParams->_pointSize);
	_shader_depth->setFloat("z_near", _camera->getZNear());
	_shader_depth->setFloat("z_far", _camera->getZFar());
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // only write to depth buffer
	_scene->draw();

	// Attribute Blend pass
	_shader_attr->use();
	_shader_attr->setMat4("view_proj_matrix", proj * view);
	_shader_attr->setFloat("fov_y", _camera->getFovY());
	_shader_attr->setVec2("viewport_size", glm::vec2(_camera->getWidth(), _camera->getHeight()));
	_shader_attr->setFloat("epsilon", epsilon);
	_shader_attr->setFloat("particle_radius", _renderingParams->_pointSize);
	_shader_attr->setFloat("z_near", _camera->getZNear());
	_shader_attr->setFloat("z_far", _camera->getZFar());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _gDepthTex);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // write to color buffers again
	glDepthMask(GL_FALSE);                           // but not to depth buffer
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	_scene->draw();

	// Rendering pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(_renderingParams->_backgroundColor.x, _renderingParams->_backgroundColor.y, _renderingParams->_backgroundColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	switch (_renderingParams->_displayMode)
	{
	case (int)DisplayMode::DEPTH:
		_shader_render_depth->use();
		_shader_render_depth->setFloat("z_far", _camera->getZFar());
		_shader_render_depth->setFloat("z_near", _camera->getZNear());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _gDepthTex);
		break;
	case (int)DisplayMode::COLORS:
		_shader_render_colors->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _gColorTex);
		break;
	case (int)DisplayMode::NORMALS:
		_shader_render_normals->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _gNormalTex);
		break;
	case (int)DisplayMode::FULL:
		glm::vec4 ld_view = view * glm::vec4(_renderingParams->_lightDirection, 0.0);
		_shader_render_full->use();
		_shader_render_full->setVec3("light_dir_view_spc", glm::vec3(ld_view));
		_shader_render_full->setVec3("light_color", _renderingParams->_lightColor);
		_shader_render_full->setFloat("ambient_strength", _renderingParams->_ambient_strength);
		_shader_render_full->setFloat("specular_strength", _renderingParams->_specular_strength);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _gNormalTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _gColorTex);
		break;
	}

	glDisable(GL_BLEND);
	glBindVertexArray(_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::createSceneFromFile(const std::string& filename)
{
	spdlog::info("Loading point cloud file : {} ", filename);
	_scene = std::make_unique<PointCloudScene>(filename);
	_scene->initialize();
}

void Renderer::sizeChanged(const uint16_t width, const uint16_t height)
{
	// recreate texture / buffers
	setupGBuffers();
	// update camera
	_camera->sizeChanged(width, height);
	// tell opengl
	glViewport(0, 0, width, height);
}