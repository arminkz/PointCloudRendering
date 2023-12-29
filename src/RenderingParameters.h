#pragma once
#include "stdafx.h"

enum class DisplayMode : int
{
	DEPTH,
	COLORS,
	NORMALS,
	FULL
};

struct RenderingParameters
{
public:
	int _displayMode;

	float _pointSize;
	float _epsilonMultiplier;

	glm::vec3 _backgroundColor;

	//Lighting
	glm::vec3 _lightDirection;
	glm::vec3 _lightColor;
	float _ambient_strength;
	float _specular_strength;

	bool _cpuCulling;
	bool _cpuCullingEveryFrame;

	//Default values
	RenderingParameters() :
		_displayMode((int)DisplayMode::FULL),
		_pointSize(0.01f),
		_epsilonMultiplier(0.25),
		_backgroundColor(0.4f, 0.4f, 0.4f),

		_lightDirection(glm::normalize(glm::vec3(-0.5f, -1.0f, 0.4f))),
		_lightColor(1.0f, 1.0f, 1.0f),
		_ambient_strength(0.5f),
		_specular_strength(0.2f),

		_cpuCulling(false),
		_cpuCullingEveryFrame(false)
	{}
};



