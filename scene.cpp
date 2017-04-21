#include "scene.h"
#include "pathfinding.h"

using namespace std;
using namespace cv;

vector<Vec3d> point_cloud;
vector<node> lista;

int flaga2 =2;
int flaga=0;

float mx,my;
Vec3d temp1, temp2;

void setProjection(int w1, int h1)
{
	float ratio;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	ratio = 1.0f * w1 / h1;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
        glViewport(0, 0, w1, h1);

	// Set the clipping volume
	gluPerspective(45,ratio,0.1,1000);
	glMatrixMode(GL_MODELVIEW);
}

void changeSize(int w1,int h1) {

	if(h1 == 0)
		h1 = 1;

	// we're keeping these values cause we'll need them latter
	w = w1;
	h = h1;

	// set subwindow 1 as the active window
	glutSetWindow(subWindow1);
	// resize and reposition the sub window
	glutPositionWindow(border,border);
	glutReshapeWindow(w-2*border, h/2 - border*3/2);
	setProjection(w-2*border, h/2 - border*3/2);

	// set subwindow 2 as the active window
	glutSetWindow(subWindow2);
	// resize and reposition the sub window
	glutPositionWindow(border,(h+border)/2);
	glutReshapeWindow(w/2-border*3/2, h/2 - border*3/2);
	setProjection(w/2-border*3/2,h/2 - border*3/2);

	// set subwindow 3 as the active window
	glutSetWindow(subWindow3);
	// resize and reposition the sub window
	glutPositionWindow((w+border)/2,(h+border)/2);
	glutReshapeWindow(w/2-border*3/2,h/2 - border*3/2);
	setProjection(w/2-border*3/2,h/2 - border*3/2);
}

void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {

	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void restorePerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();

	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, w, h, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

// Common Render Items for all subwindows
void renderScene2() {

// Draw ground
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
                glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(10.0f, 0.0f, 0.0f);
        glEnd();

        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINES);
                glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 10.0f, 0.0f);
        glEnd();

        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
                glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 10.0f);
        glEnd();

        /*glColor3f(0.3f, 0.5f, 1.0f);
        glBegin(GL_LINES);
                glVertex3f(temp1[0], temp1[1], temp1[2]);
		glVertex3f(temp2[0], temp2[1], temp2[2]);
        glEnd();*/

        //if(!point_cloud.empty())
        //{
            glPointSize(1.0);
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_POINTS);
            for(unsigned int i =0; i < point_cloud.size(); i++ )
            {
                glVertex3f(point_cloud[i][0], point_cloud[i][1], point_cloud[i][2]);
            }

            for (int i = -8; i < 8; i++ )
            {
                for (int j = -8; j < 8; j++ )
                {
                    for (int  k= -8; k < 8; k++ )
                    {
                        glVertex3f(0.029 + 0.001*i, 1.718 + 0.001*j, 0.870 + k*0.001);
                    }
                }
            }

            glEnd();
        //}

        if(!lista.empty())
        {
            glColor3f(0.0f, 1.0f, 1.0f);
            glBegin(GL_LINE_STRIP);
            for(unsigned int i =0; i < lista.size(); i++ )
            {
                glVertex3f(lista[i].worldPosition[0], lista[i].worldPosition[1], lista[i].worldPosition[2]);
            }
            glEnd();
        }



}

// Display func for main window
void renderScene() {
	glutSetWindow(mainWindow);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

// Display func for sub window 1
void renderScenesw1() {

	glutSetWindow(subWindow1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(x, y, z,
		  x + lx,y + ly,z + lz,
		  0.0f,1.0f,0.0f);

	renderScene2();

	// display fps in the top window
 	frame++;

	time1=glutGet(GLUT_ELAPSED_TIME);
	if (time1 - timebase > 1000) {
		sprintf(s,"Lighthouse3D - FPS:%4.2f",
			frame*1000.0/(time1-timebase));
		timebase = time1;
		frame = 0;
	}

	setOrthographicProjection();

	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(5,30,0,GLUT_BITMAP_HELVETICA_12,s);
	glPopMatrix();

	restorePerspectiveProjection();

	glutSwapBuffers();
}

// Display func for sub window 2
void renderScenesw2() {

	glutSetWindow(subWindow2);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(x, y+6, z,
		  x ,y - 1,z,
		  lx,0,lz);

	// Draw red cone at the location of the main camera
	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	glTranslatef(x,y,z);
	glRotatef(180-(angle+deltaAngle)*180.0/3.14,0.0,1.0,0.0);
	glutSolidCone(0.1,0.4f,2,2);
	glPopMatrix();

	renderScene2();

	glutSwapBuffers();
}

// Display func for sub window 3
void renderScenesw3() {

	glutSetWindow(subWindow3);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(x-lz*8 , y, z+lx*8,
		  x ,y ,z ,
		  0.0f,1.0f,0.0f);

	// Draw red cone at the location of the main camera
	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	glTranslatef(x,y,z);
	glRotatef(180-(angle+deltaAngle)*180.0/3.14,0.0,1.0,0.0);
	glutSolidCone(0.1,0.4f,4,4);
	glPopMatrix();

	renderScene2();

	glutSwapBuffers();
}

// Global render func
void renderSceneAll() {

	// check for keyboard movement
	if (deltaMove) {
		computePos(deltaMove);
		glutSetWindow(mainWindow);
		glutPostRedisplay();
	}

	renderScene();
	renderScenesw1();
	renderScenesw2();
	renderScenesw3();
}

// -----------------------------------
//             KEYBOARD
// -----------------------------------

void processNormalKeys(unsigned char key, int xx, int yy) {

	if (key == 27) {
		glutDestroyWindow(mainWindow);
		exit(0);
	}
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_UP : deltaMove = 0.5f; break;
		case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
	}
	glutSetWindow(mainWindow);
	glutPostRedisplay();

}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN : deltaMove = 0;break;
	}
}

// -----------------------------------
//             MOUSE
// -----------------------------------

Vec3d GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    return Vec3d(posX, posY, posZ);
}

unsigned int find(Vec3d input)
{
    for(unsigned int i=0; i< point_cloud.size(); i++)
    {
        if(point_cloud[i] == input)
    {
            cout << "znaleziono"<< endl;
            return i;
    }
    }
    return 65535;
}

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


void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);

		glutSetWindow(mainWindow);
		glutPostRedisplay();
	}
}



void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			deltaAngle = 0.0f;
			xOrigin = -1;

		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}else if (button == GLUT_RIGHT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
                    flaga=1;
                    mx = x;
                    my=y;
		}
		else  {// state = GLUT_DOWN

		}
	}
}


void update()
{
    Vec3d real;
    if(flaga == 1)
    {
        real = round(GetOGLPos(mx, my));
        cout << real << endl;
        cout << find(real) << endl;
        if(find(real) != 65535)
        {
            flaga2++;
            if (flaga2 == 1)
            {
                temp1 = real;
            }
            else if(flaga2 == 2)
            {
                temp2 = real;
                cout << "Wybrano" << endl;
            }

        }
        flaga =0;
    }
}

// -----------------------------------
//             MAIN and INIT
// -----------------------------------

void init() {

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// register callbacks
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
        glutIdleFunc(update);
}