#include "Node.hpp"

Node::Node()
{
    parent = NULL;
}

Node::Node(bool _walkable, cv::Vec3f _worldPosition, int _gridX, int _gridY, int _gridZ)
{
    walkable = _walkable;
    worldPosition = _worldPosition;
    gridX = _gridX;
    gridY = _gridY;
    gridZ = _gridZ;
    parent = NULL;
}

int Node::fCost()
{
    return gCost + hCost;
}

bool Node::operator==(const Node& n1)
{
    return worldPosition == n1.worldPosition;
}

bool Node::operator!=(const Node& n1)
{
    return !(*this==n1);
}

bool Node::isWalkable()
{
    return walkable;
}
