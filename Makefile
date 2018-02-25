CPPINCLUDE_DIRS =  -I. -I/usr/local/include/ -I/usr/include/eigen3/
CPPLIBS = -L/usr/local/lib/opencv -lopencv_core -lopencv_flann -lopencv_video
LIBS =

CPP = g++ -std=c++11
CPPFLAGS = -std=c++11 -Wall $(CPPINCLUDE_DIRS)

CPPSOURCE =$(wildcard main.cpp shader.cpp window3d.cpp Grid.cpp Node.cpp)
CPPOUTFILE = path_finding
CPPOBJS = $(CPPSOURCE:.cpp=.o)

all: $(CPPOUTFILE)

$(CPPOUTFILE): $(CPPOBJS)
		$(CPP) $(CPPFLAGS) $(CPPOBJS) -o $(CPPOUTFILE) -lGL -lGLU -lglut -lglfw -lGLEW `pkg-config --libs opencv` $(CPPLIBS)

clean:
		rm -f *.o $(CPPOUTFILE)
