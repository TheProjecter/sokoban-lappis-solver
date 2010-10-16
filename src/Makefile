#
#KTH - Royal Institute of Technology
#DD2380 - Artificial Intelligence
#Autumn 2010
#
#Makefile for the simple sokoban solver
#
#@author  Kevin Anceau <anceau@kth.se>
#@author  Andrea Baisero <baisero@kth.se>
#@author  Carlos Alberto Colmenares <cacol@kth.se>
#@author  Manuel Parras Ruiz De Azea <mprda@kth.se>
#

SOURCES=\
	sokoban-lappis-solver.cpp \
	soko_node.cpp \
	client.cpp \
	deadlock_check.cpp \
	soko_algorithm.cpp \
	soko_heuristic.cpp \
	hash_table.cpp \
	hungarian.c \
	hungarian_tc.cpp

default:
	g++ $(SOURCES) -o sokoban-lappis-solver

clean:
	rm sokoban-lappis-solver
