#include "Octree.h"


void Octree::calculateChildAABB(AABB& out, Octant octant, AABB parentRegion)
{
    glm::vec3 center = parentRegion.center();
    if (octant == Octant::O1) {
        out = AABB(center, parentRegion.max());
    }
    else if (octant == Octant::O2) {
        out = AABB(glm::vec3(parentRegion.min().x, center.y, center.z), glm::vec3(center.x, parentRegion.max().y, parentRegion.max().z));
    }
    else if (octant == Octant::O3) {
        out = AABB(glm::vec3(parentRegion.min().x, parentRegion.min().y, center.z), glm::vec3(center.x, center.y, parentRegion.max().z));
    }
    else if (octant == Octant::O4) {
        out = AABB(glm::vec3(center.x, parentRegion.min().y, center.z), glm::vec3(parentRegion.max().x, center.y, parentRegion.max().z));
    }
    else if (octant == Octant::O5) {
        out = AABB(glm::vec3(center.x, center.y, parentRegion.min().z), glm::vec3(parentRegion.max().x, parentRegion.max().y, center.z));
    }
    else if (octant == Octant::O6) {
        out = AABB(glm::vec3(parentRegion.min().x, center.y, parentRegion.min().z), glm::vec3(center.x, parentRegion.max().y, center.z));
    }
    else if (octant == Octant::O7) {
        out = AABB(parentRegion.min(), center);
    }
    else if (octant == Octant::O8) {
        out = AABB(glm::vec3(center.x, parentRegion.min().y, parentRegion.min().z), glm::vec3(parentRegion.max().x, center.y, center.z));
    }
}


Octree::Node::Node(std::vector<PointModel> objects, AABB aabb, unsigned int depth)
    : region(aabb), activeOctants(0x0), depth(depth)
{
    this->objects.insert(this->objects.end(), objects.begin(), objects.end());

    parent = nullptr;
    for (int i = 0; i < 8; i++) children[i] = nullptr;
}


void Octree::Node::build()
{

    //spdlog::info("building node at depth {}", depth);

    // Calculate Average
    glm::vec3 sumPosition(0.f);
    glm::vec3 sumColor(0.f);
    for (int i = 0; i < objects.size(); i++) {
        PointModel o = objects[i];
        sumPosition += o.position;
        sumColor += o.color;
    }
    averageObject = PointModel(sumPosition / (float)objects.size(), sumColor / (float)objects.size());
    //spdlog::info("average set!");
    
    // Termination condition
    if (objects.size() <= MAX_LEAF_OBJECTS) {
        return;
    }

    // Create Child AABBs
    AABB childAABBs[8];
    for (int i = 0; i < 8; i++) {
        calculateChildAABB(childAABBs[i], (Octant)(1 << i), region);
    }
    //spdlog::info("child AABB set !");

    //Determine which octant to place object
    std::vector<PointModel> childObjs[8]; // array of object vectors
    int len = objects.size();
    for (int i = 0; i < len; i++) {
        //if (i % 1000 == 0) spdlog::info("points remaining : {}", len);
        PointModel o = objects[i];
        for (int j = 0; j < 8; j++) {
            if (childAABBs[j].contains(o.position)) {
                childObjs[j].push_back(o);
                break;
                //objects.erase(objects.begin() + i);
                //i--;
                //len--;
            }
        }
    }
    objects.clear();
    //spdlog::info("child points set!");

    //Create childs
    for (int i = 0; i < 8; i++) {
        if (childObjs[i].size() > 0) {
            children[i] = new Node(childObjs[i], childAABBs[i], depth+1);

            //Activate in bitmap
            activeOctants |= (1 << i);

            children[i]->parent = this;
            children[i]->build(); // can build merged into constructor ?
        }
    }
}

