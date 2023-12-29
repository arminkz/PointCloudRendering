#include "PointCloudScene.h"
#include "stdafx.h"

#include "loaders/PLYLoader.h"
#include "RenderingParameters.h"
#include "Renderer.h"

//Constructor
PointCloudScene::PointCloudScene(PointCloud pointcloud)
	: _pointcloud(pointcloud), _aggregator(std::make_unique<PointCloudAggregator>())
{
    _aggregator->initialize(pointcloud);
}

//Constructor loading from file
PointCloudScene::PointCloudScene(const std::string& filename) :
    _aggregator(std::make_unique<PointCloudAggregator>())
{
	_pointcloud = PLYLoader::load(filename);
    _aggregator->initialize(_pointcloud);
}

//Loads VBO/VAO
void PointCloudScene::initialize() 
{
    std::vector<PointModel> aggregatedPoints;

    if (Renderer::getInstance()->getRenderingParameters()->_cpuCulling) {
        _aggregator->aggregate();
        aggregatedPoints = _aggregator->getAggregatedPoints();
        if (aggregatedPoints.size() == 0) return;
    }
    else {
        aggregatedPoints = _pointcloud.points;
    }

    glGenBuffers(1, &_vbo_id);

    glGenVertexArrays(1, &_vao_id);
    glBindVertexArray(_vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);
    glBufferData(GL_ARRAY_BUFFER, aggregatedPoints.size() * sizeof(PointModel), &aggregatedPoints[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointModel), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PointModel), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}


void PointCloudScene::draw() 
{
    std::vector<PointModel> aggregatedPoints;

    if (Renderer::getInstance()->getRenderingParameters()->_cpuCulling) {
        if (Renderer::getInstance()->getRenderingParameters()->_cpuCullingEveryFrame) {
            initialize();
        }
        aggregatedPoints = _aggregator->getAggregatedPoints();
        if (aggregatedPoints.size() == 0) return;
    }
    else {
        aggregatedPoints = _pointcloud.points;
    }

	glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, aggregatedPoints.size() * sizeof(PointModel), &aggregatedPoints[0]);
	glBindVertexArray(_vao_id);
	glDrawArrays(GL_POINTS, 0, (GLsizei)aggregatedPoints.size());
}