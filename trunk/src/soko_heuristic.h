/**
 * KTH - Royal Institute of Technology
 * DD2380 - Artificial Intelligence
 * Autumn 2010
 *
 * Simple sokoban solver
 *
 * @author  Kevin Anceau <anceau@kth.se>
 * @author  Andrea Baisero <baisero@kth.se>
 * @author  Carlos Alberto Colmenares <cacol@kth.se>
 * @author  Manuel Parras Ruiz De Azea <mprda@kth.se>
 *
 */
#ifndef _SOKO_HEURISTIC_H_
#define _SOKO_HEURISTIC_H_

#include<iostream>
#include<string.h>
using namespace std;

//This file is splitted in two parts:
// 1.- Calc of min distances
// 2.- Calc of heuristic
// 3.- Others (auxiliary functions)

//............................................................................
//                            Calc of Min distances
//............................................................................

/**
 * Calculates the minimum distance from every cell to
 * any other using the manhattan distance.
 *
 * @param num_cells         The number of cells in the rel representation
 *                          of the board
 *
 * @param rel_to_abs_table  Transformation table from relative
 *                          positions to absolute ones.
 *
 * @param board_width       The width of the board
 * 
 * @return                  A "num_cells x num_cells" matrix, represented
 *                          as an array, where the position [i][j] contains
 *                          the manhattan distance between the cells i and j
 */
int* manhattan_dist( int num_cells,
                     int *rel_to_abs_table,
                     int board_width 
                     );

//............................................................................
//                            Calc of heuristic
//............................................................................

/**
 * Function that calculates the heuristic of a node to the
 * goals by adding the distance of each box to the closest
 * goal
 *
 * @param num_boxes             The number of boxes on the board
 *
 * @param num_goals             The number og goals on the board
 *
 * @param box_goal_distance     A matrix "num_boxes x num_goals" represented
 *                              as an array. The cell [i][j] of the matrix
 *                              represents the shortest distance between
 *                              box i and goal j on the board
 *
 * @return                      The heuristic: it sums the minimum distance of
 *                              each node to its closest goal
 */
int nearest_goal( int num_boxes, int num_goals,
                    int *box_goal_distance);

//............................................................................
//                          Others (auxiliary functions)
//............................................................................

#endif

