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
    Vec3d worldPosition;
    int gridX;
    int gridY;
    int gridZ;

    unsigned int gCost;
    unsigned int hCost;
    node *parent;

    node();
    node(bool _walkable, Vec3d _worldPosition, int _gridX, int _gridY, int _gridZ );

    int fCost();
    bool operator==(const node& n1);
    bool operator!=(const node& n1);
};

class Grid
{
public:
    int gridSizeX, gridSizeY, gridSizeZ;
    static const double nodeSize = 0.001;
    Vec3d begin, end, gridWoldSize;
    vector<vector<vector<bool> > > grid;
    Grid(Vec3d _begin, Vec3d _end);
    vector<node> GetNeighbours(node N);
    unsigned int GetX(Vec3d worldPosition);
    unsigned int GetY(Vec3d worldPosition);
    unsigned int GetZ(Vec3d worldPosition);
    Vec3d WorldPointFromNode(unsigned int x, unsigned int y, unsigned int z);
};



vector<node> RetracePath(node startNode, node endNode);
vector<node> FindPath(Vec3d startPos, Vec3d targetPos, Grid &grid);
int GetDistance(node n1, node n2);
extern vector<Vec3d> point_cloud;
extern vector<node> lista;



#endif