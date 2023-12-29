#pragma once
#include "stdafx.h"

struct PointModel {
    glm::vec3 position;
    glm::vec3 color;

    PointModel();
    PointModel(glm::vec3 position, glm::vec3 color);
};