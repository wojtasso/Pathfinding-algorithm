// Include GLFW
#include <stdio.h>
#include <GLFW/glfw3.h>
extern GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
    return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3(5, 5, 5);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = -3.14f/2.0f;
// Initial Field of View
float FoV = 60.0f;

float speed = 0.1f; // 3 units / second
float mouseSpeed = 0.001f;

double xpos = 1024.0f/2.0f, ypos = 1024.0f/2.0f;
double xPrev = 1024.0f/2.0f, yPrev = 1024.0f/2.0f;

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        /*Toggle mouse pointer visibility */
        int mode = glfwGetInputMode(window, GLFW_CURSOR);
        glfwSetInputMode(window, GLFW_CURSOR, mode ^ 0x01);
    }

}


void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    FoV -= yoffset;
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
