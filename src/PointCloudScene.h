#pragma once
#include "stdafx.h"

#include "model/PointModel.h"
#include "model/PointCloud.h"
#include "RenderingParameters.h"
#include "PointCloudAggregator.h"

class PointCloudScene {
protected:
	PointCloud _pointcloud;
	std::unique_ptr<PointCloudAggregator> _aggregator;

	unsigned int _vao_id;
	unsigned int _vbo_id;

public:
	PointCloudScene(PointCloud points);
	PointCloudScene(const std::string& filename);

	void initialize();
	void draw();
};