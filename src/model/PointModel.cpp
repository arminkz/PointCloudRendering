#include "PointModel.h"

PointModel::PointModel()
	: position(0.f), color(0.f) {}

PointModel::PointModel(glm::vec3 position, glm::vec3 color)
	: position(position), color(color) {}