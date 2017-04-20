#include "pathfinding.h"

using namespace std;
using namespace cv;


node::node()
{
    parent = NULL;
}

node::node(bool _walkable, Vec3d _worldPosition, int _gridX, int _gridY, int _gridZ )
{
        walkable = _walkable;
        worldPosition = _worldPosition;
        gridX = _gridX;
        gridY = _gridY;
        gridZ = _gridZ;
        parent = NULL;
}

int node::fCost()
{
    return gCost + hCost;
}

bool node::operator==(const node& n1)
{
    return worldPosition == n1.worldPosition;
}

bool node::operator!=(const node& n1)
{
        return !(*this==n1);
}

Grid::Grid(Vec3d _begin, Vec3d _end)
{
    Vec3d increase(nodeSize, nodeSize, nodeSize);
    begin = _begin;
    end = _end + increase;
    gridWoldSize[0] = abs(end[0] - begin[0]);
    gridWoldSize[1] = abs(end[1] - begin[1]);
    gridWoldSize[2] = abs(end[2] - begin[2]);
    gridSizeX = (int)(gridWoldSize[0]/nodeSize);
    gridSizeY = (int)(gridWoldSize[1]/nodeSize);
    gridSizeZ = (int)(gridWoldSize[2]/nodeSize);

    grid.resize(gridSizeX);
    for ( int i = 0; i < gridSizeX; i++)
    {
        grid[i].resize(gridSizeY);
        for ( int j = 0; j < gridSizeY; j++)
            grid[i][j].resize(gridSizeZ);
    }

    cout << "Tworzenie siatki ..." << endl;

    for ( int i = 0; i < gridSizeX; i++)
    {
        for ( int j = 0; j < gridSizeY; j++)
        {
            for ( int k = 0; k < gridSizeZ; k++)
            {
                //bool walkable = (find(WorldPointFromNode(i, j, k)) == 65535) ? false : true;
                grid[i][j][k] = true;
            }
        }
    }

    cout << "Uzupelnanie ..." << endl;
    for (unsigned int i = 0; i < point_cloud.size(); i++ )
    {
        grid[GetX(point_cloud[i])][GetY(point_cloud[i])][GetZ(point_cloud[i])] = false;
    }

    cout << "Gotowe" << endl;
    for (int x = -8; x < 8; x++ )
    {
        for (int y = -8; y < 8; y++ )
        {
            for (int  z= -8; z < 8; z++ )
            {
                Vec3d tmp(0.029 + 0.001*x,1.718 + 0.001*y, 0.870 + z*0.001 );
                grid[GetX(tmp)][GetY(tmp)][GetZ(tmp)] = false;
            }
        }
    }
}

vector<node> Grid::GetNeighbours(node N)
{
    vector<node> neighbours;

    for (int x = -1; x <= 1; x++ )
    {
        for(int y = -1; y <= 1; y++)
        {
            for(int z = -1; z <= 1; z++)
            {
                if(x==0 && y==0 && z==0)
                    continue;

                int checkX = N.gridX + x;
                int checkY = N.gridY + y;
                int checkZ = N.gridZ + z;

                if (checkX >= 0 && checkX < gridSizeX && checkY >= 0 && checkY < gridSizeY && checkZ >= 0 && checkZ < gridSizeZ)
                {
                    node tmp(grid[checkX][checkY][checkZ], WorldPointFromNode(checkX, checkY, checkZ), checkX, checkY, checkZ);
                    neighbours.push_back(tmp);
                }
            }
        }
    }
    return neighbours;
}

unsigned int Grid::GetX(Vec3d worldPosition)
{
    double percentX = (worldPosition[0] - begin[0]) / gridWoldSize[0];
    int x = rint(gridSizeX * percentX);
    return x;
}

unsigned int Grid::GetY(Vec3d worldPosition)
{
    double percentY = (worldPosition[1] - begin[1]) / gridWoldSize[1];
    int y = rint(gridSizeY * percentY);
    return y;
}

unsigned int Grid::GetZ(Vec3d worldPosition)
{
        double percentZ = (worldPosition[2] - begin[2]) / gridWoldSize[2];
        int z = rint(gridSizeZ * percentZ);
        return z;
}

Vec3d Grid::WorldPointFromNode(unsigned int x, unsigned int y, unsigned int z)
{
    Vec3d worldPosition(begin[0] + x*nodeSize, begin[1] + y*nodeSize, begin[2] + z*nodeSize);
    return worldPosition;
}


vector<node> FindPath(Vec3d startPos, Vec3d targetPos, Grid &grid)
{
    node startNode(true, startPos, grid.GetX(startPos), grid.GetY(startPos), grid.GetZ(startPos));
    node targetNode(true, targetPos, grid.GetX(targetPos), grid.GetY(targetPos), grid.GetZ(targetPos));
    grid.grid[grid.GetX(startPos)][ grid.GetY(startPos)][grid.GetZ(startPos)] = true;
    grid.grid[grid.GetX(targetPos)][grid.GetY(targetPos)][grid.GetZ(targetPos)] = true;
    startNode.gCost = 0;
    startNode.hCost = GetDistance(startNode, targetNode);
    //targetNode.gCost = GetDistance(targetNode, startNode);
    //targetNode.hCost = 0;

    vector<node> openSet;
    vector<node> closedSet;
    openSet.push_back(startNode);
    vector<node> neighbours;
    vector <node> all_nodes;
    all_nodes.resize(36777216);
    int idx = 0;
    while (openSet.size() > 0)
    {
        node currentNode = openSet[0];
        for (unsigned int i = 0; i < openSet.size(); i++)
        {
            if ((openSet[i].fCost() < currentNode.fCost()) || (openSet[i].fCost() == currentNode.fCost() && openSet[i].hCost < currentNode.hCost))
            {
                currentNode = openSet[i];
            }
        }

        openSet.erase(remove(openSet.begin(), openSet.end(), currentNode), openSet.end());
        closedSet.push_back(currentNode);
        cout << idx << " " << currentNode.worldPosition << " Hcost = " << currentNode.hCost << " FCost " << currentNode.fCost() << endl;

        if (currentNode == targetNode || currentNode.hCost == 0)
        {
            cout << currentNode.worldPosition << " " << targetNode.worldPosition << endl;
            return RetracePath(startNode, currentNode);
        }


        all_nodes[idx] = currentNode;
        vector<node> neighbours = grid.GetNeighbours(currentNode);
        size_t neighbours_count = grid.GetNeighbours(currentNode).size();
        for(unsigned int i =0 ; i< neighbours_count; i++)
        {
            node neighbour = neighbours[i];
            if (!(neighbour.walkable))
            {
                cout << " Przeszkoda " << endl;
                //cin.ignore();
            }
            if (!(neighbour.walkable) || (find(closedSet.begin(), closedSet.end(), neighbour) != closedSet.end()))
            {
                continue;
            }

            unsigned int newMovementCostToNeighbour = currentNode.gCost + GetDistance(currentNode, neighbour);
            if (newMovementCostToNeighbour < neighbour.gCost || !(find(openSet.begin(), openSet.end(), neighbour) != openSet.end() ))
            {
                neighbour.gCost = newMovementCostToNeighbour;
                neighbour.hCost = GetDistance(neighbour, targetNode);
                neighbour.parent = &all_nodes[idx];

                if(!(find(openSet.begin(), openSet.end(), neighbour) != openSet.end()))
                {
                    openSet.push_back(neighbour);
                }
            }
        }
        idx++;
    }
    //Return empty vector in case of error
    openSet.clear();
    cout << "Pusto" << endl;
    return openSet;
}




int GetDistance(node n1, node n2)
{
    int dstX = abs(n1.gridX - n2.gridX);
    int dstY = abs(n1.gridY - n2.gridY);
    int dstZ = abs(n1.gridZ - n2.gridZ);

    int smallest = min(dstX, min(dstY, dstZ));
    int greatest = max(dstX, max(dstY, dstZ));
    int middle;

    if(greatest == dstX)
    {
        if(smallest == dstY)
            middle = dstZ;
        else if(smallest == dstZ)
            middle = dstY;
    }
    else if(greatest == dstY)
    {
        if(smallest == dstX)
            middle = dstZ;
        else if(smallest == dstZ)
            middle = dstX;
    }
    else if(greatest == dstZ)
    {
        if(smallest == dstX)
            middle = dstY;
        else if(smallest == dstY)
            middle = dstX;
    }

    return (17*smallest + 14*(middle - smallest) + 10*(greatest - middle));
}

vector<node> RetracePath(node startNode, node endNode)
{
    vector<node> path;
    node currentNode = endNode;
    while(currentNode!=startNode)
    {
        path.push_back(currentNode);
        currentNode = (*currentNode.parent);
    }
    reverse(path.begin(), path.end());
    return path;
}