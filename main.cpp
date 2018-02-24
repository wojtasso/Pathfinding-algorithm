#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "pathfinding.h"
#include "window3d.hpp"

using namespace std;
using namespace cv;

Vec3d round(Vec3d input)
{
    //Vec3i tmp = rint(input * 1000);
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

    /*      int menu=0;
            while(!(menu == 1 || menu ==2)) {
            cout << "Wybierz scenariusz 1 lub 2 " << endl;
            cin >> menu;
            }*/

    vector<Vec3f> points;

    cout << "Odczytywanie punktow ..." << endl;
    vector<Mat> points3d_estimated;
    FileStorage fs("myfile.txt",FileStorage::READ);
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
    vector<double> X_axis, Y_axis, Z_axis;

    for (unsigned int i =0; i < points.size(); i++) {
        X_axis.push_back(points[i][0]);
        Y_axis.push_back(points[i][1]);
        Z_axis.push_back(points[i][2]);
    }

    vector<double>::iterator
        result_X = min_element(X_axis.begin(), X_axis.end()),
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
    cout << "Find Path ..." << endl;
    cout << "Grid_begin " << Grid_begin << endl;

    Vec3f firstPos(0.090, 0.280, 0.810);
    node firstNode(true, firstPos, obj.GetX(firstPos), obj.GetY(firstPos), obj.GetZ(firstPos));
    Vec3f secondPos(-1.014, -0.021, 1.639);
    node secondNode(true, secondPos, obj.GetX(secondPos), obj.GetY(secondPos), obj.GetZ(secondPos));
    cout << "end " <<  secondPos << endl;

    cout << obj.gridSizeX << " " << obj.gridSizeY << " " << obj.gridSizeZ << endl;
    cout << obj.GetX(secondPos) << " " << obj.GetY(secondPos) << " " << obj.GetZ(secondPos) << endl;

    vector<node> path = FindPath(firstPos, secondPos, obj);
    path.insert(path.begin(), firstNode);

    init3DWindow(points, path);
    loop3DWindow(points.size(), path.size());
    close3DWindow();
    return 0;
}
