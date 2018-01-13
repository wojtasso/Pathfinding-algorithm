#ifndef CONTROLS_HPP
#define CONTROLS_HPP
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

#endif
