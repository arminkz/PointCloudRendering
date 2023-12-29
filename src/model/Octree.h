#pragma once
#include "stdafx.h"

#include "model/AABB.h"
#include "model/PointModel.h"


namespace Octree {

    static const int MAX_LEAF_OBJECTS = 5;

    enum class Octant : unsigned char {
        O1 = 0x01,	// = 0b00000001
        O2 = 0x02,	// = 0b00000010
        O3 = 0x04,	// = 0b00000100
        O4 = 0x08,	// = 0b00001000
        O5 = 0x10,	// = 0b00010000
        O6 = 0x20,	// = 0b00100000
        O7 = 0x40,	// = 0b01000000
        O8 = 0x80	// = 0b10000000
    };

    void calculateChildAABB(AABB& out, Octant octant, AABB parentRegion);
       
    class Node {
    public:
        Node* parent;
        Node* children[8];

        unsigned int depth;
        unsigned char activeOctants; //bitmap

        AABB region;

        std::vector<PointModel> objects;
        PointModel averageObject;

        Node(std::vector<PointModel> objects, AABB aabb, unsigned int depth = 0);

        void build();
    };

}