#ifndef _SCENE_H
#define _SCENE_H
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <GL/glut.h>

using namespace cv;
using namespace std;

void setProjection(int w1, int h1);

void changeSize(int w1,int h1);

void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string);

void restorePerspectiveProjection();

void setOrthographicProjection();

void computePos(float deltaMove);
void renderScene2();
void renderScene();

void renderScenesw1();
void renderScenesw2();
void renderScenesw3();
void renderSceneAll();
void processNormalKeys(unsigned char key, int xx, int yy);

void pressKey(int key, int xx, int yy);
void releaseKey(int key, int x, int y);
Vec3d GetOGLPos(int x, int y);

unsigned int find(Vec3d input);
Vec3d round(Vec3d input);
void mouseMove(int x, int y);
void mouseButton(int button, int state, int x, int y);
void update();
void init();


// angle of rotation for the camera direction
extern float angle;

// actual vector representing the camera's direction
extern float lx,lz, ly;

// XZ position of the camera
extern float x, z, y;

// the key states. These variables will be zero
//when no key is being presses
extern float deltaAngle;
extern float deltaMove;
extern int xOrigin;

// width and height of the window
extern int h,w;

// variables to compute frames per second
extern int frame;
extern long time1, timebase;
extern char s[50];

// variables to hold window identifiers
extern int mainWindow, subWindow1,subWindow2,subWindow3;
//border between subwindows
extern int border;

extern int flaga2;
extern int flaga;
extern float mx,my;
extern Vec3d temp1, temp2;


#endif