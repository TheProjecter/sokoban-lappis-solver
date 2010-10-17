KTH - Royal Institute of Technology
Artificial Intelligence DD2380
Autumn 2010

Anceau Kévin               <anceau@kth.se> 
Andrea Baisero             <baisero@kth.se>
Carlos Colmenares          <cacol@kth.se>
Manuel Parras Ruiz De Azúa <mprda@kth.se>

Sokoban Project
.....................................................................
                         README FILE
.....................................................................

I) Compiling

A makefile for compiling the source files has been provided.

To compile the program, it's only necessary to get inside the "code"
folder and use the included Makefile.:

    > cd code
    > make

The resulting executable file will be named "sokoban-lappis-solver"

.............

If the user doesn't have "GNU make" installed in his computer, he
can use the next comands instead:

    > cd code
    > g++ sokoban-lappis-solver.cpp soko_node.cpp client.cpp
      deadlock_check.cpp soko_algorithm.cpp soko_heuristic.cpp
      hash_table.cpp hungarian_tc.cpp -o sokoban-lappis-solver -O3

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

II) Executing the program

    Usage: ./sokoban-lappis-solver num_board

To execute the program only one argument must be passed to the
executable:

    num_board:       An integer, the number of the sokoban board
                     for which we want to find the solution

Example executions:

    > ./sokoban-lappis-solver 52
    > ./sokoban-lappis-solver 126

