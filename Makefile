CXX = g++
CXXFLAGS = -std=c++11
CPPFLAGS = -I. `pkg-config --cflags opencv eigen3 glew glfw3`
LDFLAGS = `pkg-config --libs opencv eigen3 glew glfw3`

CPPOUTFILE = path_finding
CPPOBJS = $(CPPSOURCE:.cpp=.o)
CPPSOURCE = main.cpp shader.cpp window3d.cpp Grid.cpp Node.cpp

all: $(CPPOUTFILE)

$(CPPOUTFILE): $(CPPSOURCE)
	$(CXX) $(CXXFLAGS) $(CPPSOURCE) -o $(CPPOUTFILE) $(CPPFLAGS) $(LDFLAGS)
clean:
	rm -f *.o $(CPPOUTFILE)
