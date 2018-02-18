#
# Basic OpenCV example tested on Ubuntu
#

CPPINCLUDE_DIRS =  -I. -I/usr/local/include/ -I/usr/include/eigen3/
CPPLIBS = -L/usr/local/lib/opencv -lopencv_core -lopencv_flann -lopencv_video
LIBS =

CPP = g++
CFLAGS =  -Wall -c -I.
CPPFLAGS = -Wall $(CPPINCLUDE_DIRS)
LFLAGS = -Wall

CPPSOURCE =$(wildcard main.cpp shader.cpp window3d.cpp pathfinding.cpp)
CPPOUTFILE = path_finding
CPPOBJS = $(CPPSOURCE:.cpp=.o)

all: $(CPPOUTFILE)

$(CPPOUTFILE): $(CPPOBJS)
		$(CPP) $(CPPFLAGS) $(CPPOBJS) -o $(CPPOUTFILE) -lGL -lGLU -lglut -lglfw -lGLEW `pkg-config --libs opencv` $(CPPLIBS)

clean:
		rm -f *.o $(CPPOUTFILE)
