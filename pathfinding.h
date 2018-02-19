#ifndef _PATHFINDING_H
#define _PATHFINDING_H

#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fenv.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class node
{
public:
    bool walkable;
    Vec3f worldPosition;
    int gridX;
    int gridY;
    int gridZ;

    unsigned int gCost;
    unsigned int hCost;
    node *parent;

    node();
    node(bool _walkable, Vec3f _worldPosition, int _gridX, int _gridY, int _gridZ );

    int fCost();
    bool operator==(const node& n1);
    bool operator!=(const node& n1);
};

class Grid
{
public:
    int gridSizeX, gridSizeY, gridSizeZ;
    static const double nodeSize = 0.001;
    Vec3f begin, end, gridWoldSize;
    vector<vector<vector<bool> > > grid;
    Grid(Vec3f _begin, Vec3f _end, vector<Vec3f> &point_cloud);
    vector<node> GetNeighbours(node N);
    unsigned int GetX(Vec3f worldPosition);
    unsigned int GetY(Vec3f worldPosition);
    unsigned int GetZ(Vec3f worldPosition);
    Vec3f WorldPointFromNode(unsigned int x, unsigned int y, unsigned int z);
};



vector<node> RetracePath(node startNode, node endNode);
vector<node> FindPath(Vec3f startPos, Vec3f targetPos, Grid &grid);
int GetDistance(node n1, node n2);



#endif
