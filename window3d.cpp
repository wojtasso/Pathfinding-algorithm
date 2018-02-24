// Include GLFW
#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iterator>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include "window3d.hpp"
#include "shader.hpp"
#include "Node.hpp"

using namespace glm;
using std::cout;
using std::endl;
using std::vector;

GLFWwindow* window;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

GLuint programID;
GLuint MatrixID;
GLuint axisBuffer;

GLuint colorBuffer;
GLuint axisBufferID;

bool dragFlag = false;

// Initial position : on +Z
glm::vec3 position = glm::vec3(5, 5, 5);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = -3.14f/2.0f;
// Initial Field of View
float FoV = 60.0f;

float speed = 0.1f; // 0.1 units / second
float mouseSpeed = 0.001f;

double xpos = 1024.0f/2.0f, ypos = 1024.0f/2.0f;
double xPrev = 1024.0f/2.0f, yPrev = 1024.0f/2.0f;

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


glm::mat4 getViewMatrix()
{
    return ViewMatrix;
}

glm::mat4 getProjectionMatrix()
{
    return ProjectionMatrix;
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        //Calculate mouse ray
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        int w, h;
        glfwGetWindowSize(window, &w, &h);

        double norm_x = (2.0f*x) / w - 1;
        double norm_y = (2.0f*y) / h - 1;
        norm_y =  -1.0f * norm_y;
        glm::mat4 inv = glm::inverse(ProjectionMatrix);
        glm::vec4 clipCoords(norm_x, norm_y, -1.0f, 1.0f);
        glm::vec4 eyeCords = (inv * clipCoords);
        eyeCords[2] = -1.0f;
        eyeCords[3] = 0.0f;
        glm::mat4 invertedView = glm::inverse(ViewMatrix);
        glm::vec4 ray = invertedView * eyeCords;
        glm::vec3 mouseRay(ray[0], ray[1], ray[2]);
        mouseRay = glm::normalize(mouseRay);

        cout << "Ray: " << mouseRay[0] << " " << mouseRay[1] << " " << mouseRay[2] << endl;
        //Calculate 3d coordinates
        get3DPos(x, y);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        xPrev = xpos;
        yPrev = ypos;
        dragFlag = true;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        dragFlag = false;
    }
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    FoV -= yoffset;
}

void init3DWindow(vector<cv::Vec3f> &vertex, vector<Node> &path)
{
    /* Initialise GLFW */
    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        exit(EXIT_FAILURE);
    }

    int glfwMajor, glfwMinor, glfwRev;
    glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRev);
    cout << "GLFW version: " << glfwMajor << "." << glfwMinor << "."
        << glfwRev << endl;
    /* Enable Multisample anti-aliasing (MSAA), 4* MSAA */
    glfwWindowHint(GLFW_SAMPLES, 4);
    /* Set API version to OpenGL 3.3 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    /* Open a window and create its OpenGL context */
    window = glfwCreateWindow((mode->width * 2)/3, (mode->height * 2)/3,
            "Pathfinding", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    /* Initialize GLEW */
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /* Ensure we can capture the escape key being pressed below */
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwPollEvents();
    glfwSetCursorPos(window, (mode->width * 2)/2.0f, (mode->height * 2)/2.0f);

    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    /* Dark blue background */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glGenBuffers(1, &axisBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, axisBuffer);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(g_vertex_buffer_data) + path.size()*sizeof(cv::Vec3f) + vertex.size()*sizeof(cv::Vec3f),
            0,
            GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER,
            0,
            sizeof(g_vertex_buffer_data),
            g_vertex_buffer_data);

    for (unsigned int i = 0; i < path.size(); i++) {
        glBufferSubData(GL_ARRAY_BUFFER,
                sizeof(g_vertex_buffer_data) + i*sizeof(cv::Vec3f),
                sizeof(cv::Vec3f),
                &path[i].worldPosition);
    }

    glBufferSubData(GL_ARRAY_BUFFER,
            sizeof(g_vertex_buffer_data) + path.size()*sizeof(cv::Vec3f),
            vertex.size() * sizeof(cv::Vec3f),
            (void*)&vertex[0][0]);

    //White color for found path
    GLfloat whitePath[3] = { 1.0f, 1.0f, 1.0f };

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(g_color_buffer_data) + path.size()*sizeof(whitePath) + vertex.size()*sizeof(cv::Vec3f),
            0,
            GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,
            0,
            sizeof(g_color_buffer_data),
            g_color_buffer_data);

    for (unsigned int i = 0; i < path.size(); i++) {
        glBufferSubData(GL_ARRAY_BUFFER,
                sizeof(g_color_buffer_data) + i*sizeof(whitePath),
                sizeof(whitePath),
                (void*)&whitePath[0]);
    }

    // Fill points with green color
    for (unsigned int i = 0; i < vertex.size(); i++) {
        GLfloat green[] = {0.0f, 1.0f, 0.0f};
        glBufferSubData(GL_ARRAY_BUFFER,
                sizeof(g_color_buffer_data) + path.size()*sizeof(whitePath) + i*sizeof(green),
                sizeof(green),
                (void*)&green[0]);
    }

    glGenVertexArrays(1, &axisBufferID);
    glBindVertexArray(axisBufferID);

    programID = LoadShaders("SimpleTransform.vertexshader",
            "ColorFragmentShader.fragmentshader");
    MatrixID = glGetUniformLocation(programID, "MVP");
}

void loop3DWindow(unsigned int pointsSize, unsigned int pathSize)
{
    do{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        computeMatricesFromInputs();
        ProjectionMatrix = getProjectionMatrix();
        ViewMatrix = getViewMatrix();
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, axisBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        /* Color */
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_LINES, 0, 6);
        glDrawArrays(GL_LINE_STRIP, 6, pathSize);
        glDrawArrays(GL_POINTS, 6 + pathSize, pointsSize);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        /* Swap buffers */
        glfwSwapBuffers(window);
        glfwPollEvents();

    } /* Check if the ESC key was pressed or the window was closed */
    while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0 );
}

void close3DWindow()
{
    // Cleanup VBO
    glDeleteBuffers(1, &axisBuffer);
    glDeleteBuffers(1, &colorBuffer);
    glDeleteVertexArrays(1, &axisBufferID);
    glDeleteProgram(programID);

    /* Close OpenGL window and terminate GLFW */
    glfwTerminate();
}

void get3DPos(double x, double y)
{
    GLint viewport[4];
    GLfloat winX, winY, winZ;

    glGetIntegerv( GL_VIEWPORT, viewport );

    glm::vec4 ViewPort(viewport[0], viewport[1], viewport[2], viewport[3]);
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;

    glReadPixels(x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    glm::vec3 pos;
    pos = unProject( glm::vec3(winX, winY, winZ), ViewMatrix, ProjectionMatrix, ViewPort);

    cout << pos[0] << ", " << pos[1] << ", " << pos[2] << endl;
    //return Vec3d(posX, posY, posZ);
}



void computeMatricesFromInputs()
{
    // Get mouse position
    glfwGetCursorPos(window, &xpos, &ypos);

    // Compute new orientation
    if (dragFlag) {
        horizontalAngle += mouseSpeed * float(xpos - xPrev);
        verticalAngle   += mouseSpeed * float(ypos - yPrev);
        xPrev = xpos;
        yPrev = ypos;
    }

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
            );

    // Right vector
    glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14f/2.0f),
            0,
            cos(horizontalAngle - 3.14f/2.0f)
            );

    // Up vector
    glm::vec3 up = glm::cross( right, direction );

    // Move forward
    if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
        position += direction * speed;
    }
    // Move backward
    if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
        position -= direction * speed;
    }
    // Strafe right
    if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
        position += right * speed;
    }
    // Strafe left
    if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        position -= right * speed;
    }

    // Projection matrix : Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix = glm::lookAt(
            position,
            position+direction,
            up);
}
