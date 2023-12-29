#include "PointCloudAggregator.h"

#include "Renderer.h"

PointCloudAggregator::PointCloudAggregator() 
{
}

void PointCloudAggregator::initialize(PointCloud pc)
{
	spdlog::info("Creating Octree ...");
	_world_size = glm::length(pc.aabb.max() - pc.aabb.min());
	//Save point cloud into Octree
	_octree = std::make_unique<Octree::Node>(pc.points, pc.aabb);
	_octree->build();
	spdlog::info("Octree Done!");
	aggregate();
}

void PointCloudAggregator::aggregate() {
	_aggregated.clear();

	//View dependent
	Camera* cam = Renderer::getInstance()->getCamera();
	glm::vec3 eye = cam->getEye();
	glm::mat4 mvp = cam->getProjectionMatrix() * cam->getViewMatrix();

	traverseOctree(_octree.get(), 15, mvp, eye);

	spdlog::info("Aggregated count: {}", _aggregated.size());
}


void PointCloudAggregator::traverseOctree(Octree::Node* node, unsigned int maxDepth, const glm::mat4& mvp, const glm::vec3& eye) {

	if ((node->depth == maxDepth) || (node->activeOctants == 0)) {
		//Leaf Node or max depth reached. extract points
		if (node->objects.size() > 0) {
			//Leaf Node
			_aggregated.insert(_aggregated.end(), node->objects.begin(), node->objects.end());
		}
		else {
			//Max depth reached use average
			_aggregated.push_back(node->averageObject);
		}
		return;
	}

	for (int i = 0; i < 8; i++) {
		if (node->activeOctants & (1 << i)) {

			//Frustum Culling
			if (!node->children[i]->region.testAgainstFrustum(mvp)) continue;

			float dist = glm::length(node->children[i]->region.center() - eye);
			if (dist > _world_size / 2) {
				traverseOctree(node->children[i], 9, mvp, eye);
			}
			else if (_world_size / 4 < dist < _world_size / 2) {
				traverseOctree(node->children[i], 10, mvp, eye);
			}
			else {
				traverseOctree(node->children[i], 15, mvp, eye);
			}
		}
	}
}



