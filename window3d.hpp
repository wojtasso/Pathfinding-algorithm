#ifndef WINDOW3D_HPP
#define WINDOW3D_HPP
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <opencv2/opencv.hpp>
#include "Node.hpp"

class window3d {

    public:
        window3d(std::vector<cv::Vec3f> &vertex, std::vector<Node> &path);
        ~window3d();
        void loop3DWindow(unsigned int pointsSize, unsigned int pathSize);
    private:
        void computeMatricesFromInputs();
        void get3DPos(double x, double y);
        void scrollCallback(double xoffset, double yoffset);
        void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

        GLFWwindow* window;
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        GLuint programID;
        GLuint MatrixID;
        GLuint axisBuffer;
        GLuint colorBuffer;
        GLuint axisBufferID;

        bool dragFlag;
        glm::vec3 position = glm::vec3(5, 5, 5);
        float horizontalAngle = 3.14f;
        float verticalAngle = -3.14f/2.0f;
        float FoV = 60.0f;
        float speed = 0.1f; // 0.1 units / second
        float mouseSpeed = 0.001f;
        double xpos = 1024.0f/2.0f, ypos = 1024.0f/2.0f;
        double xPrev = 1024.0f/2.0f, yPrev = 1024.0f/2.0f;


};


#endif
