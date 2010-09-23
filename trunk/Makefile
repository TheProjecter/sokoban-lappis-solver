#Makefile for the sokoban-lappis-solver

#C++ Compiler name
CPP_C=g++

#Flags for the compiler
CPP_FLAGS=-O3 -I lib

#All sources must be in the /src directory
SOURCES=\
	sokoban-lappis-solver.cpp \
	soko_node.cpp

SOURCES_WITH_PATH=$(SOURCES:%=$(CAELPATH)/src/%) 

BINARIES=$(SOURCES:%.cpp=bin/%.o) 

default: $(BINARIES)
	$(CPP_C) $(CPP_FLAGS) -o sokoban-lappis-solver

bin/%.o: src/%.cpp
	$(CPP_C) $(CPP_FLAGS) $< -c -o bin/$*.o

clean:
	rm bin/* sokoban-lappis-solver

