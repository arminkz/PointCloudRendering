#pragma once
#include "stdafx.h"

#include "model/PointCloud.h"
#include "model/PointModel.h"

namespace PLYLoader {

	auto load(const std::string& filename) -> PointCloud;

}