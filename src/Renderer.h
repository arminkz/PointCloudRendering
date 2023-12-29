#pragma once
#include "stdafx.h"

#include "utilities/Singleton.h"
#include "RenderingParameters.h"
#include "PointCloudScene.h"
#include "Shader.h"
#include "Camera.h"

class Renderer : public Singleton<Renderer>
{
	friend class Singleton<Renderer>;

protected:

	std::unique_ptr<PointCloudScene> _scene;
	std::unique_ptr<RenderingParameters> _renderingParams;

	// Texture Shaders
	std::unique_ptr<Shader> _shader_depth;
	std::unique_ptr<Shader> _shader_attr;

	// Render Shaders
	std::unique_ptr<Shader> _shader_render_full;
	std::unique_ptr<Shader> _shader_render_depth;
	std::unique_ptr<Shader> _shader_render_colors;
	std::unique_ptr<Shader> _shader_render_normals;

	// Buffer and Texture IDs
	unsigned int _gFrameBuffer, _gDepthTex, _gNormalTex, _gColorTex;
	unsigned int _quadVAO;

	// Camera (View / Projection Matrix)
	std::unique_ptr<Camera> _camera;

	Renderer();

	void setupShaders();

	void setupGBuffers();

	GLuint setupRenderQuad();

public:

	virtual ~Renderer();

	void initialize();

	void render();

	void createSceneFromFile(const std::string& filename);

	void sizeChanged(const uint16_t width, const uint16_t height);

	RenderingParameters* getRenderingParameters() { return _renderingParams.get(); }

	PointCloudScene* getCurrentScene() { return _scene.get(); }

	Camera* getCamera() { return _camera.get(); }

};