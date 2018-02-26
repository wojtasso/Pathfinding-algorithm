#ifndef _GRID_HPP
#define _GRID_HPP

#include <opencv2/opencv.hpp>
#include "Node.hpp"

class Grid
{
    public:
        Grid(cv::Vec3f _begin, cv::Vec3f _end, std::vector<cv::Vec3f> &point_cloud);
        unsigned int getX(cv::Vec3f worldPosition);
        unsigned int getY(cv::Vec3f worldPosition);
        unsigned int getZ(cv::Vec3f worldPosition);
        std::vector<Node> findPath(cv::Vec3f startPos, cv::Vec3f targetPos);
    private:
        const double nodeSize = 0.001f;
        cv::Vec3f begin, end, gridWoldSize;
        int gridSizeX, gridSizeY, gridSizeZ;
        std::vector<std::vector<std::vector<bool> > > grid;
        cv::Vec3f worldPointFromNode(unsigned int x, unsigned int y, unsigned int z);
        std::vector<Node> getNeighbours(Node N);
        int getDistanceDiagonal(Node n1, Node n2);
        int getDistanceManhattan(Node n1, Node n2);
        int getDistanceChebyshev(Node n1, Node n2);
        int getDistanceEucliden(Node n1, Node n2);
        std::vector<Node> retracePath(Node startNode, Node endNode);
};

#endif
