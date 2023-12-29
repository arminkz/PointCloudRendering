#pragma once
#include "stdafx.h"

#include "PointModel.h"
#include "AABB.h"

class PointCloud
{
public:
	AABB aabb;
	std::vector<PointModel> points;
};
