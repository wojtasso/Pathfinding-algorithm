#include "Grid.hpp"
#include "Node.hpp"

using std::vector;
using std::cout;
using std::endl;

Grid::Grid(cv::Vec3f _begin, cv::Vec3f _end, vector<cv::Vec3f> &point_cloud)
{
    cout << "Tworzenie siatki ..." << endl;
    cv::Vec3f increase(nodeSize, nodeSize, nodeSize);
    begin = _begin;
    end = _end + increase;
    gridWoldSize[0] = std::abs(end[0] - begin[0]);
    gridWoldSize[1] = std::abs(end[1] - begin[1]);
    gridWoldSize[2] = std::abs(end[2] - begin[2]);
    gridSizeX = (int)(gridWoldSize[0]/nodeSize);
    gridSizeY = (int)(gridWoldSize[1]/nodeSize);
    gridSizeZ = (int)(gridWoldSize[2]/nodeSize);

    grid.resize(gridSizeX);
    for (int i = 0; i < gridSizeX; i++)
    {
        grid[i].resize(gridSizeY);
        for (int j = 0; j < gridSizeY; j++)
            grid[i][j].resize(gridSizeZ, true); //All nodes walkable
    }

    cout << "Uzupelnanie ..." << endl;
    for (unsigned int i = 0; i < point_cloud.size(); i++)
    {
        grid[GetX(point_cloud[i])][GetY(point_cloud[i])][GetZ(point_cloud[i])] = false;
    }
    cout << "End " << endl;
}

vector<Node> Grid::GetNeighbours(Node N)
{
    vector<Node> neighbours;

    for (int x = -1; x <= 1; x++ ) {
        for (int y = -1; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                if(x==0 && y==0 && z==0)
                    continue;

                int checkX = N.gridX + x;
                int checkY = N.gridY + y;
                int checkZ = N.gridZ + z;

                if (checkX >= 0 && checkX < gridSizeX &&
                        checkY >= 0 &&
                        checkY < gridSizeY &&
                        checkZ >= 0 && checkZ < gridSizeZ) {
                    Node tmp(grid[checkX][checkY][checkZ],
                            WorldPointFromNode(checkX, checkY, checkZ),
                            checkX, checkY, checkZ);
                    neighbours.push_back(tmp);
                }
            }
        }
    }
    return neighbours;
}

unsigned int Grid::GetX(cv::Vec3f worldPosition)
{
    double percentX = (worldPosition[0] - begin[0]) / gridWoldSize[0];
    int x = rint(gridSizeX * percentX);
    return x;
}

unsigned int Grid::GetY(cv::Vec3f worldPosition)
{
    double percentY = (worldPosition[1] - begin[1]) / gridWoldSize[1];
    int y = rint(gridSizeY * percentY);
    return y;
}

unsigned int Grid::GetZ(cv::Vec3f worldPosition)
{
    double percentZ = (worldPosition[2] - begin[2]) / gridWoldSize[2];
    int z = rint(gridSizeZ * percentZ);
    return z;
}

cv::Vec3f Grid::WorldPointFromNode(unsigned int x, unsigned int y, unsigned int z)
{
    cv::Vec3f worldPosition(begin[0] + x*nodeSize,
            begin[1] + y*nodeSize,
            begin[2] + z*nodeSize);
    return worldPosition;
}


vector<Node> Grid::FindPath(cv::Vec3f startPos, cv::Vec3f targetPos)
{
    Node startNode(true,
            startPos,
            GetX(startPos),
            GetY(startPos),
            GetZ(startPos));
    Node targetNode(true,
            targetPos,
            GetX(targetPos),
            GetY(targetPos),
            GetZ(targetPos));

    grid[GetX(startPos)][GetY(startPos)][GetZ(startPos)] = true;
    grid[GetX(targetPos)][GetY(targetPos)][GetZ(targetPos)] = true;
    startNode.gCost = 0;
    startNode.hCost = GetDistance(startNode, targetNode);

    vector<Node> openSet;
    vector<Node> closedSet;
    openSet.push_back(startNode);
    vector<Node> neighbours;
    vector<Node> all_nodes;
    all_nodes.resize(36777216);
    int idx = 0;
    while (openSet.size() > 0)
    {
        Node currentNode = openSet[0];
        for (unsigned int i = 0; i < openSet.size(); i++) {
            if ((openSet[i].fCost() < currentNode.fCost()) ||
                (openSet[i].fCost() == currentNode.fCost() && openSet[i].hCost < currentNode.hCost))
            {
                currentNode = openSet[i];
            }
        }

        openSet.erase(remove(openSet.begin(), openSet.end(), currentNode), openSet.end());
        closedSet.push_back(currentNode);
        cout << idx << " " << currentNode.worldPosition <<
            " Hcost = " << currentNode.hCost <<
            " FCost " << currentNode.fCost() << endl;

        if (currentNode == targetNode || currentNode.hCost == 0) {
            cout << currentNode.worldPosition << " " << targetNode.worldPosition << endl;
            return RetracePath(startNode, currentNode);
        }


        all_nodes[idx] = currentNode;
        vector<Node> neighbours = GetNeighbours(currentNode);
        size_t neighbours_count = GetNeighbours(currentNode).size();
        for (unsigned int i = 0 ; i < neighbours_count; i++)
        {
            Node neighbour = neighbours[i];
            if (!(neighbour.walkable) || (find(closedSet.begin(), closedSet.end(), neighbour) != closedSet.end()))
                continue;

            unsigned int newMovementCostToNeighbour =
                currentNode.gCost + GetDistance(currentNode, neighbour);
            if (newMovementCostToNeighbour < neighbour.gCost ||
                    !(find(openSet.begin(), openSet.end(), neighbour) != openSet.end()))
            {
                neighbour.gCost = newMovementCostToNeighbour;
                neighbour.hCost = GetDistance(neighbour, targetNode);
                neighbour.parent = &all_nodes[idx];

                if(!(find(openSet.begin(), openSet.end(), neighbour) != openSet.end()))
                    openSet.push_back(neighbour);
            }
        }
        idx++;
    }
    //Return empty vector in case of error
    openSet.clear();
    cout << "Pusto" << endl;
    return openSet;
}

int Grid::GetDistance(Node n1, Node n2)
{
    int dstX = abs(n1.gridX - n2.gridX);
    int dstY = abs(n1.gridY - n2.gridY);
    int dstZ = abs(n1.gridZ - n2.gridZ);

    int smallest = std::min(dstX, std::min(dstY, dstZ));
    int greatest = std::max(dstX, std::max(dstY, dstZ));
    int middle;

    if(greatest == dstX) {
        if(smallest == dstY)
            middle = dstZ;
        else if(smallest == dstZ)
            middle = dstY;
    } else if(greatest == dstY) {
        if(smallest == dstX)
            middle = dstZ;
        else if(smallest == dstZ)
            middle = dstX;
    } else if(greatest == dstZ) {
        if(smallest == dstX)
            middle = dstY;
        else if(smallest == dstY)
            middle = dstX;
    }

    return (17*smallest + 14*(middle - smallest) + 10*(greatest - middle));
}

vector<Node> Grid::RetracePath(Node startNode, Node endNode)
{
    vector<Node> path;
    Node currentNode = endNode;
    while(currentNode!=startNode) {
        path.push_back(currentNode);
        currentNode = (*currentNode.parent);
    }
    reverse(path.begin(), path.end());
    return path;
}
