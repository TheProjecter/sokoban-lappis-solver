/**
 * KTH - Royal Institute of Technology
 * DD2380 - Artificial Intelligence
 * Autumn 2010
 *
 * Search algorithms used for finding a solution
 * in the sokoban-solver
 *
 * @author  Kevin Anceau <anceau@kth.se>
 * @author  Andrea Baisero <baisero@kth.se>
 * @author  Carlos Alberto Colmenares <cacol@kth.se>
 * @author  Manuel Parras Ruiz De Azea <mprda@kth.se>
 *
 */
#ifndef _SOKO_ALGORITHM_H_
#define _SOKO_ALGORITHM_H_

#include<queue>
#include"soko_node.h"
#include"deadlock_check.h"

using namespace std;

/**
 * Takes initial soko_node and does a breadth-first search
 * until it finds a solution.
 *
 * @param init_node         Starting node of the Sokoban problem
 *
 * @param board_height      The height of the board.
 *
 * @param board_width       The width of the board.
 *
 * @param abs_to_rel_table  Transformation table from absolute
 *                          positions to relative ones.
 *
 * @param rel_to_abs_table  Transformation table from relative
 *                          positions to absolute ones.
 *
 * @param deadlock_list     List of deadlock cells
 *
 * @param num_cells         Total amount of cells on the board
 *
 * @param neighbors         A matrix with the neighboring cells
 *                          of every cell in the relative representation
 *
 * @param num_neighbors     An array with the number of neighboring
 *                          cells of each cell in the relative
 *                          representation
 *
 * @param goals_pos         This pointer will hold the goal
 *                          positions
 *
 * @return                  A soko_node in which all boxes are in the goals
 */
soko_node* breadth_first_search(soko_node *init_node, int board_height,
                                int board_width,
                                int* abs_to_rel_table, int *rel_to_abs_table,
                                int* deadlock_list, int num_cells,
                                int (*neighbors)[4], int *num_neighbors,
                                int *goals_pos);

/**
 * Takes initial soko_node and does a breadth-first search
 * until it finds a solution.
 *
 * @param init_node         Starting node of the Sokoban problem
 *
 * @param board_height      The height of the board.
 *
 * @param board_width       The width of the board.
 *
 * @param abs_to_rel_table  Transformation table from absolute
 *                          positions to relative ones.
 *
 * @param rel_to_abs_table  Transformation table from relative
 *                          positions to absolute ones.
 *
 * @param deadlock_list     List of deadlock cells
 *
 * @param num_cells         Total amount of cells on the board
 *
 * @param neighbors         A matrix with the neighboring cells
 *                          of every cell in the relative representation
 *
 * @param num_neighbors     An array with the number of neighboring
 *                          cells of each cell in the relative
 *                          representation
 *
 * @param goals_pos         This pointer will hold the goal
 *                          positions
 *
 * @param goals_rel_pos     An array with all the relative positions of
 *                          the goals
 *
 * @param num_boxes         The number of boxes on the board
 *
 * @param num_goals         The number of goals on the board
 *
 * @return                  A soko_node in which all boxes are in the goals
 */
soko_node* a_star_search(soko_node *init_node, int board_height,
								int board_width,
                                int* abs_to_rel_table, int *rel_to_abs_table,
                                int* deadlock_list, int num_cells,
                                int (*neighbors)[4], int *num_neighbors,
                                int *goals_pos, int *goals_rel_pos,
                                int num_boxes, int num_goals);



#endif

