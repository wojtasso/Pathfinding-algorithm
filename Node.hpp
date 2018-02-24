#ifndef _NODE_HPP
#define _NODE_HPP

#include <opencv2/opencv.hpp>

class Node
{
    public:
        Node();
        Node(bool _walkable, cv::Vec3f _worldPosition, int _gridX, int _gridY, int _gridZ);
        int fCost();
        bool operator==(const Node& n1);
        bool operator!=(const Node& n1);
        cv::Vec3f worldPosition;
        int gridX;
        int gridY;
        int gridZ;
        unsigned int gCost;
        unsigned int hCost;
        Node *parent;
        bool isWalkable();
        bool walkable;

};

#endif
