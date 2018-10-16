cmake_minimum_required (VERSION 3.5)

set (CMAKE_CXX_STANDARD 11)
SET("OpenCV_DIR" "/home/wojtek/opencv-3.4.1/build")

find_package(OpenCV 3.4 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
set(ADD_LIBS -lglfw)
add_definitions ("-Wall")
include_directories(${OpenCV_INCLUDE_DIRS}
                    ${OpenGL_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS}
                )

add_executable (path_finding main.cpp shader.cpp window3d.cpp Grid.cpp Node.cpp)

target_link_libraries(path_finding ${OpenCV_LIBS} ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} ${ADD_LIBS})
