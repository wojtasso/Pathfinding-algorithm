#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

//#include "pathfinding.h"
#include "window3d.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv) {

        init3DWindow();
        loop3DWindow();
        close3DWindow();
        return 0;
        /*int menu=0;
        while(!(menu == 1 || menu ==2))
        {
            cout << "Wybierz scenariusz 1 lub 2 " << endl;
            cin >> menu;
        }

        cout << "Odczytywanie punktow ..." << endl;
        vector<Mat> points3d_estimated;
        FileStorage fs("myfile.txt",FileStorage::READ);
        fs["points3d"] >> points3d_estimated;

        for (unsigned int i = 0; i < points3d_estimated.size(); i++)
        {
            point_cloud.push_back(round(Vec3d(points3d_estimated[i])));
        }

        //Nowy uklad
        for (unsigned int i = 0; i < point_cloud.size(); i++)
        {
            point_cloud[i][0]= point_cloud[i][0]*-1;
            point_cloud[i][1]= point_cloud[i][1]*-1 + 1.5;
            point_cloud[i][2]= point_cloud[i][2];
        }
        //Create Grid
        vector<double> X_axis, Y_axis, Z_axis;

        for (unsigned int i =0; i< point_cloud.size(); i++)
        {
            X_axis.push_back(point_cloud[i][0]);
            Y_axis.push_back(point_cloud[i][1]);
            Z_axis.push_back(point_cloud[i][2]);
        }
        vector<double>::iterator
            result_X = min_element(X_axis.begin(), X_axis.end()),
            result_Y = min_element(Y_axis.begin(), Y_axis.end()),
            result_Z = min_element(Z_axis.begin(), Z_axis.end());

        Vec3d Grid_begin(X_axis[distance(X_axis.begin(), result_X)], Y_axis[distance(Y_axis.begin(), result_Y)], Z_axis[distance(Z_axis.begin(), result_Z)]);

        result_X = max_element(X_axis.begin(), X_axis.end());
        result_Y = max_element(Y_axis.begin(), Y_axis.end());
        result_Z = max_element(Z_axis.begin(), Z_axis.end());

        Vec3d Grid_end(X_axis[distance(X_axis.begin(), result_X)], Y_axis[distance(Y_axis.begin(), result_Y)], Z_axis[distance(Z_axis.begin(), result_Z)]);

        if(menu==1)
        {
            point_cloud.clear();
            Grid obj(Grid_begin, Grid_end);

            Vec3d firstPos(-0.100, 1.680, 0.850);
            node firstNode(true, firstPos, obj.GetX(firstPos), obj.GetY(firstPos), obj.GetZ(firstPos));
            Vec3d secondPos(-0.514, 1.559, 1.039);
            node secondNode(true, secondPos, obj.GetX(secondPos), obj.GetY(secondPos), obj.GetZ(secondPos));
            cout << "end " <<  secondPos << endl;

            cout << obj.gridSizeX << " " << obj.gridSizeY << " " << obj.gridSizeZ << endl;
            cout << obj.GetX(secondPos) << " " << obj.GetY(secondPos) << " " << obj.GetZ(secondPos) << endl;

            vector <node> tmpe = FindPath(firstPos, secondPos, obj);
            tmpe.insert(tmpe.begin(), firstNode);
            lista = tmpe;

            temp1= firstPos;
            temp2 = secondPos;

        }
        else if(menu == 2)
        {
            Grid obj(Grid_begin, Grid_end);
            cout << "Find Path ..." << endl;
            cout << "Grid_begin " << Grid_begin << endl;

            Vec3d firstPos(0.090, 1.780, 0.810);
            node firstNode(true, firstPos, obj.GetX(firstPos), obj.GetY(firstPos), obj.GetZ(firstPos));
            Vec3d secondPos(-1.014, 1.479, 1.639);
            node secondNode(true, secondPos, obj.GetX(secondPos), obj.GetY(secondPos), obj.GetZ(secondPos));
            cout << "end " <<  secondPos << endl;

            cout << obj.gridSizeX << " " << obj.gridSizeY << " " << obj.gridSizeZ << endl;
            cout << obj.GetX(secondPos) << " " << obj.GetY(secondPos) << " " << obj.GetZ(secondPos) << endl;

            vector <node> tmpe = FindPath(firstPos, secondPos, obj);
            tmpe.insert(tmpe.begin(), firstNode);
            lista = tmpe;

            temp1= firstPos;
            temp2 = secondPos;
        }

	// init GLUT and create main window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	mainWindow = glutCreateWindow("A* ");

	// callbacks for main window
	glutDisplayFunc(renderSceneAll);
	glutReshapeFunc(changeSize);

	// Removing the idle function to save CPU and GPU
	//glutIdleFunc(renderSceneAll);
	init();

	// sub windows
	subWindow1 = glutCreateSubWindow(mainWindow, border,border,w-2*border, h/2 - border*3/2);
	glutDisplayFunc(renderScenesw1);
	init();

	subWindow2 = glutCreateSubWindow(mainWindow, border,(h+border)/2,w/2-border*3/2, h/2 - border*3/2);
	glutDisplayFunc(renderScenesw2);
	init();

	subWindow3 = glutCreateSubWindow(mainWindow, (w+border)/2,(h+border)/2,w/2-border*3/2,h/2 - border*3/2);
	glutDisplayFunc(renderScenesw3);
	init();

	// enter GLUT event processing cycle
	glutMainLoop();*/

	return 0;
}
