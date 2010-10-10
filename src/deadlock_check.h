#ifndef _DEADLOCK_CHECK_H_
#define _DEADLOCK_CHECK_H_

#include "sokoban-lappis-solver.h"
#include "soko_node.h"

/* Determine which cell are deadlocks. Boxes should never be put
 * on these cells.
 *
 * @param rel_to_abs_table  Translation table, same as usual.
 *
 * @param abs_to_rel_table  Translation table, same as usual.
 *
 * @param deadlock_list     List we will fill in with index of
 *                          deadlock cells..
 *
 * @param neighbors         This pointer will end up pointing
 *                          to the neighbors matrix.
 *
 * @param num_neighbors     This pointer will end up pointing
 *                          to an array that contains the number
 *                          of neighboring cells for each cell
 *                          in the absolute representation of the
 *                          board.
 *
 * @param goals             This pointer aims at the goals' position.
 *
 * @param num_cell          Total amount of cells
 *
 * @param board_width       Size of the board.
 *
 */

void init_deadlock_list(int *rel_to_abs_table, int *abs_to_rel_table,
						int *deadlock_list, int (*neighbors)[4], 
						int *&num_neighbors, int *goals,
						int num_cell, int board_width);

/* Function used by the deadlock search at the beginning.
 * Some cells are against walls and so can be deadlocks.
 * Box beside a wall is deadlock unless there is a goal on the line,
 * or it's possible to push away from the wall
 *
 * @param rel_to_abs_table  Translation table, same as usual.
 *
 * @param deadlock_list     Deadlock cells
 *
 * @param goals             Goals' positions
 *
 * @param direction         Allow us to evaluate where are the neighbors
 *
 * @param dir               Which direction are we going ?
 *
 * @param from_cell         Cell we come from
 *
 * @param board_width       Size of the board.
 *
 */
bool init_deadlock_beside_wall(int *rel_to_abs_table,
							   int *abs_to_rel_table,
							   int *deadlock_list, 
							   int *goals,
							   int direction[2][2], int dir,
							   int from_cell, int board_width);


/* Function that evaluate if a node is a deadlock or if we
 * can continue with it. Return true if on a deadlock
 *
 * @param rel_to_abs_table  Translation table, same as usual.
 *
 * @param abs_to_rel_table  Translation table, same as usual.
 *
 * @param deadlock_list     Deadlock cells
 *
 * @param neighbors         Define the neighbors
 *
 * @param num_neighbors     Help defining the neighbors
 *
 * @param node              Evaluate a node
 *
 * @param num_cell          How many cell on the board ?
 *
 * @param board_height		Height of the board
 *
 * @param board_width		Width of the board
 *
 * @param goals_pos			Positions of the goals
 *
 */
bool is_deadlock(int *rel_to_abs_table, int *abs_to_rel_table,
                 int *deadlock_list, int (*neighbors)[4], 
                 int *&num_neighbors, soko_node *node,
                 int num_cell, int board_height, int board_width,
					int *goals_pos);

/* Function that evaluates if a node is in a freeze type deadlock
 * situation.
 * 
 * @param node				Pointer to the node
 * 
 * @param num_neighbors     Help defining the neighbors
 *
 * @param rel_to_abs_table  Translation table, same as usual.
 *
 * @param abs_to_rel_table  Translation table, same as usual.
 *
 * @param num_cell          How many cell on the board ?
 *
 * @param board_width		Width of the board
 *
 * @param goals_pos			Positions of the goals
 *
 */
bool is_freeze_deadlock(soko_node *node, int *num_neighbors,
						int *rel_to_abs_table, int *abs_to_rel_table,
						int num_cell, int board_width, int *goals_pos);

#endif
