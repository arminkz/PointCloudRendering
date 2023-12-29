#pragma once
#include "stdafx.h"

#include "model/PointCloud.h"
#include "model/PointModel.h"
#include "model/Octree.h"

class PointCloudAggregator
{

protected:

	float _world_size;

	// Octree (used to store cloud)
	std::unique_ptr<Octree::Node> _octree;
	
	// Reduced cloud
	std::vector<PointModel> _aggregated;

	void traverseOctree(Octree::Node* node, unsigned int maxDepth, const glm::mat4& mvp, const glm::vec3& eye);

public:

	PointCloudAggregator();

	void initialize(PointCloud pc);

	void aggregate();

	std::vector<PointModel> getAggregatedPoints() { return _aggregated; }
};