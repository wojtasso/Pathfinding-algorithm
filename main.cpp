#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

//#include "scene.h"
//#include "pathfinding.h"
#include "shader.hpp"
#include "controls.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLFWwindow* window;


using namespace std;
using namespace cv;


int main(int argc, char **argv) {

        /* Initialise GLFW */
	if (!glfwInit()) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
        /* Enable Multisample anti-aliasing (MSAA), 4* MSAA */
	glfwWindowHint(GLFW_SAMPLES, 4);
        /* Set API version to OpenGL 3.3 */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Open a window and create its OpenGL context */
	window = glfwCreateWindow( 1024, 768, "Pathfinding", NULL, NULL);
	if (window == NULL) {
		fprintf( stderr, "Failed to open GLFW window. If you have an "
                        "Intel GPU, they are not 3.3 compatible. Try the 2.1 "
                        "version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	/* Initialize GLEW */
        glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	/* Ensure we can capture the escape key being pressed below */
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwPollEvents();
        glfwSetCursorPos(window, 1024/2, 768/2);

	/* Dark blue background */
	glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        GLuint axisBufferID;
        glGenVertexArrays(1, &axisBufferID);
        glBindVertexArray(axisBufferID);

        GLuint programID = LoadShaders("SimpleTransform.vertexshader",
                "ColorFragmentShader.fragmentshader");
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");

        //glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f,
        //            0.1f, 100.0f);

        // Camera matrix
        /*glm::mat4 View = glm::lookAt(
            glm::vec3(10,6,6), // Camera is at (4,3,3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
            );
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 MVP = Projection * View * Model;
*/
        static const GLfloat g_vertex_buffer_data[] = {
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
        };

        static const GLfloat g_color_buffer_data[] = {
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
        };

        GLuint axisBuffer;
        glGenBuffers(1, &axisBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, axisBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
                g_vertex_buffer_data, GL_STATIC_DRAW);

        GLuint colorBuffer;
        glGenBuffers(1, &colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data),
                g_color_buffer_data, GL_STATIC_DRAW);

	do{
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(programID);

            computeMatricesFromInputs();
            glm::mat4 ProjectionMatrix = getProjectionMatrix();
            glm::mat4 ViewMatrix = getViewMatrix();
            glm::mat4 ModelMatrix = glm::mat4(1.0);
            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, axisBuffer);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            /* Color */
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            glDrawArrays(GL_LINES, 0, 6);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);

            /* Swap buffers */
            glfwSwapBuffers(window);
            glfwPollEvents();

        } /* Check if the ESC key was pressed or the window was closed */
        while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                glfwWindowShouldClose(window) == 0 );

        // Cleanup VBO
	glDeleteBuffers(1, &axisBuffer);
        glDeleteBuffers(1, &colorBuffer);
	glDeleteVertexArrays(1, &axisBufferID);
	glDeleteProgram(programID);

	/* Close OpenGL window and terminate GLFW */
	glfwTerminate();

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
