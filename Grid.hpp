#ifndef _GRID_HPP
#define _GRID_HPP

#include <opencv2/opencv.hpp>
#include "Node.hpp"

class Grid
{
    public:
        int gridSizeX, gridSizeY, gridSizeZ;
        Grid(cv::Vec3f _begin, cv::Vec3f _end, std::vector<cv::Vec3f> &point_cloud);
        unsigned int GetX(cv::Vec3f worldPosition);
        unsigned int GetY(cv::Vec3f worldPosition);
        unsigned int GetZ(cv::Vec3f worldPosition);
        std::vector<Node> FindPath(cv::Vec3f startPos, cv::Vec3f targetPos);

        std::vector<std::vector<std::vector<bool> > > grid;
    private:
        cv::Vec3f begin, end, gridWoldSize;
        cv::Vec3f WorldPointFromNode(unsigned int x, unsigned int y, unsigned int z);
        std::vector<Node> GetNeighbours(Node N);
        const double nodeSize = 0.001;
        int GetDistance(Node n1, Node n2);
        std::vector<Node> RetracePath(Node startNode, Node endNode);
};

#endif
