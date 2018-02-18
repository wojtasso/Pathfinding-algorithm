#ifndef WINDOW3D_HPP
#define WINDOW3D_HPP
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <opencv2/opencv.hpp>

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void init3DWindow(std::vector<cv::Vec3f> &vertex);
void loop3DWindow(unsigned int size);
void close3DWindow();
void get3DPos(double x, double y);
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

#endif
