#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "Node.hpp"
#include "Grid.hpp"
#include "window3d.hpp"

using std::vector;
using cv::Vec3f;
using cv::Vec3d;
using cv::Mat;

Vec3d round(Vec3d input)
{
    double vec1 = trunc(input[0] * 1000.0);
    double vec2 = trunc(input[1] * 1000.0);
    double vec3 = trunc(input[2] * 1000.0);
    vec1 = vec1/1000.0f;
    vec2 = vec2/1000.0f;
    vec3 = vec3/1000.0f;
    Vec3d output(vec1, vec2, vec3);
    return output;
}

int main(int argc, char **argv) {

    vector<Vec3f> points;
    std::cout << "Getting 3d points from the file ..." << std::endl;
    vector<Mat> points3d_estimated;
    cv::FileStorage fs("myfile.txt", cv::FileStorage::READ);
    fs["points3d"] >> points3d_estimated;

    for (unsigned int i = 0; i < points3d_estimated.size(); i++)
        points.push_back(round(Vec3d(points3d_estimated[i])));

    //Nowy uklad
    for (unsigned int i = 0; i < points.size(); i++) {
        points[i][0]= points[i][0]*-1;
        points[i][1]= points[i][1]*-1;
        points[i][2]= points[i][2];
    }

    //Create Grid
    std::vector<double> X_axis, Y_axis, Z_axis;

    for (unsigned int i =0; i < points.size(); i++) {
        X_axis.push_back(points[i][0]);
        Y_axis.push_back(points[i][1]);
        Z_axis.push_back(points[i][2]);
    }

    vector<double>::iterator result_X = min_element(X_axis.begin(), X_axis.end()),
        result_Y = min_element(Y_axis.begin(), Y_axis.end()),
        result_Z = min_element(Z_axis.begin(), Z_axis.end());

    Vec3d Grid_begin(
            X_axis[distance(X_axis.begin(), result_X)],
            Y_axis[distance(Y_axis.begin(), result_Y)],
            Z_axis[distance(Z_axis.begin(), result_Z)]);

    result_X = max_element(X_axis.begin(), X_axis.end());
    result_Y = max_element(Y_axis.begin(), Y_axis.end());
    result_Z = max_element(Z_axis.begin(), Z_axis.end());

    Vec3d Grid_end(
            X_axis[distance(X_axis.begin(), result_X)],
            Y_axis[distance(Y_axis.begin(), result_Y)],
            Z_axis[distance(Z_axis.begin(), result_Z)]);

    Grid obj(Grid_begin, Grid_end, points);
    std::cout << "Find Path ..." << std::endl;
    std::cout << "Grid_begin " << Grid_begin << std::endl;

    Vec3f firstPos(0.090, 0.280, 0.810);
    Node firstNode(true,
            firstPos,
            obj.getX(firstPos),
            obj.getY(firstPos),
            obj.getZ(firstPos));
    Vec3f secondPos(-1.014, -0.021, 1.639);
    Node secondNode(true,
            secondPos,
            obj.getX(secondPos),
            obj.getY(secondPos),
            obj.getZ(secondPos));
    std::cout << "end " <<  secondPos << std::endl;

    std::cout << obj.getX(secondPos) << " "
        << obj.getY(secondPos) << " "
        << obj.getZ(secondPos) << std::endl;

    vector<Node> path = obj.findPath(firstPos, secondPos);
    path.insert(path.begin(), firstNode);

    window3d win(points, path);
    win.loop3DWindow(points.size(), path.size());
    return 0;
}
