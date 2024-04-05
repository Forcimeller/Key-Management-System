CXX = g++
CXXFLAGS = -g -Wall -Wextra
LDFLAGS = -I/usr/local/include -L/usr/local/lib
LIBS = -lmongocxx -lbsoncxx

.PHONY : all
all : program

program: main.cpp DatabaseInterface.o
	$(CXX) $(CXXFLAGS) -o keyMan main.cpp main.h DatabaseInterface.o FileSystemInterface.o $(LDFLAGS) $(LIBS)

DatabaseInterface.o: DatabaseInterface.cpp DatabaseInterface.h
	$(CXX) $(CXXFLAGS) -c DatabaseInterface.cpp

FileSystemInterface.o: FileSystemInterface.cpp FileSystemInterface.h
	$(CXX) $(CXXFLAGS) -c FileSystemInterface.cpp

.PHONY : clean
clean:
	$(RM) *.out
	$(RM) *.o
	$(RM) keyMan