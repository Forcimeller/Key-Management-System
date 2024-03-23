CXX = g++
CXXFLAGS = -g -Wall -Wextra
LDFLAGS = -I/usr/local/include -L/usr/local/lib
LIBS = -lmongocxx -lbsoncxx

.PHONY : all
all : program

program: main.cpp
	$(CXX) $(CXXFLAGS) -o keyMan main.cpp main.h $(LDFLAGS) $(LIBS)

.PHONY : clean
clean:
	$(RM) *.out
	$(RM) keyMan