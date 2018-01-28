// Include GLFW
#include <iostream>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "window3d.hpp"
#include "shader.hpp"

using namespace glm;
using std::cout;
using std::endl;

GLFWwindow* window;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

GLuint programID;
GLuint MatrixID;
GLuint axisBuffer;
GLuint colorBuffer;
GLuint axisBufferID;

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

    10.0f, 0.0f, 0.0f,
    11.0f, 0.0f, 0.0f,
    10.0f, 0.0f, 0.0f,
    10.0f, 1.0f, 0.0f,
    10.0f, 0.0f, 0.0f,
    10.0f, 0.0f, 1.0f,

};

static const GLfloat g_color_buffer_data[] = {
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

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
    //TODO: Strange behaviour while switching from DISABLED to NORMAL
    //glfw 3.3 version will have raw mouse input
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        /*Toggle mouse pointer visibility */
        int mode = glfwGetInputMode(window, GLFW_CURSOR);
        glfwSetInputMode(window, GLFW_CURSOR, mode ^ 0x02);
    }
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    FoV -= yoffset;
}

void init3DWindow()
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();
    glfwSetCursorPos(window, (mode->width * 2)/2.0f, (mode->height * 2)/2.0f);

    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    /* Dark blue background */
    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glGenVertexArrays(1, &axisBufferID);
    glBindVertexArray(axisBufferID);


    glGenBuffers(1, &axisBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, axisBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
            g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data),
            g_color_buffer_data, GL_STATIC_DRAW);

    programID = LoadShaders("SimpleTransform.vertexshader",
            "ColorFragmentShader.fragmentshader");
    MatrixID = glGetUniformLocation(programID, "MVP");



}

void loop3DWindow()
{
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

        glDrawArrays(GL_LINES, 0, 12);

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

void computeMatricesFromInputs()
{
    // Get mouse position
    glfwGetCursorPos(window, &xpos, &ypos);

    // Compute new orientation
    horizontalAngle += mouseSpeed * float(xPrev - xpos);
    verticalAngle   += mouseSpeed * float(yPrev - ypos);
    xPrev = xpos;
    yPrev = ypos;
    //printf("H: %f V: %f \n", horizontalAngle, verticalAngle);

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
    ViewMatrix       = glm::lookAt(
            position,           // Camera is here
            position+direction, // and looks here : at the same position, plus "direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
            );
    //printf("Up: %f %f %f \n", up[0], up[1], up[2]);
    //printf("Look at %f %f %f \n", (position[0]+direction[0]), (position[1]+direction[1]), (position[2]+direction[2]));
}
