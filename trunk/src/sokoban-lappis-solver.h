#ifndef _SLS_SOKO_LAPPIS_SOLVER_H
#define _SLS_SOKO_LAPPIS_SOLVER_H

#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include<string.h>
#include<iostream>
#include"soko_node.h"

#define SPACECHAR ' '
#define WALLCHAR '#'
#define GOALCHAR '.'
#define GUYCHAR '@'
#define BOXCHAR '$'
#define BOXGOALCHAR '*'
#define GUYGOALCHAR '+'

using namespace std;

/**
 * Solve the sokoban board and return a string
 * containing the needed movements to reach for
 * a solution (Encoded as U D L R)
 *
 * @param buffer    A string containing the board
 *
 * @param buf_size  The size of the buffer
 *
 * @return          A string containing a solution
 *                  to the board
 */
char* solve_sokoban(char *buffer, int buf_size);

/**
 * Transforms a board from a c-string structure to
 * a vector<string> more familiar one.
 * OBS! The resulting board is not squared!
 *
 * @param buffer    The c-string
 *
 * @param buf_size  The size of the string
 *
 * @param board     A reference to a vector of
 *                  string where the solution will rest
 *
 * @return          The width of the board
 */
int read_board(char *buffer, int buf_size,
                vector< string > &board );

/**
 * Make all the necessary precomputations to begin the
 * search. This function computes the game board and
 * initializes the transformation tables for absolute
 * positions to relative ones and vice-versa. It also
 * initializes the initial goals positions and calculates
 * the initial position of the player and the place where
 * the boxes are.
 *
 * @param board_width       The width of the board.
 *
 * @param board             The board of the game (as a vector of
 *                          string). OBS! board is not a squared 
 *                          matrix!
 *
 * @param abs_to_rel_table  This pointer will hold the
 *                          table that transforms 
 *                          absolute positions to relative ones.
 *
 * @param rel_to_abs_table  This pointer will hold the
 *                          table that transforms 
 *                          relative positions to absolute ones.
 *
 * @param goals_pos         This pointer will hold the goal
 *                          positions
 *
 * @param init_node         This pointer will hold an initial
 *                          node with the player position
 *                          and the boxes' initial positions.
 *
 * @return                  The funtion returns the number of
 *                          cells in the relative representation
 *                          of the game.
 */
int precompute_board(
            int board_width,  vector< string > &board,
            int *&abs_to_rel_table, int *&rel_to_abs_table,
            int *&goals_pos, soko_node *&init_node
            );

/* flood the board to find which cells are inside
 * @param board             The board of the game (as a vector of
 *                          string). OBS! board is not a squared 
 *                          matrix!
 *
 * @param abs_to_rel_table  This pointer will hold the
 *                          table that transforms 
 *                          absolute positions to relative ones.
 *
 * @param rel_to_abs_table  This pointer will hold the
 *                          table that transforms 
 *                          relative positions to absolute ones.
 *
 * @param goals_pos         This pointer will hold the goal
 *                          positions
 *
 * @param box_pos           This pointer will hold the boxes
 *                          positions
 *
 * @param moves             Different moves the guy can do.
 *
 * @param board_width       Width of the board
 *
 * @param x                 (x, y) position of the guy
 *
 * @param y                 (x, y) position of the guy
 *
 * @param c                 index of the cell
 *
 */
void dfs(vector< string > &board, int *abs_to_rel_table,
	 int *rel_to_abs_table, int *goals_pos, int *box_pos,
	 const int moves[4][2], int, int, int, int &c);

inline void add_to_list(int* list, int index);
    
/**
 * This function precomputes the neighbors of
 * every cell in the relative representation of the
 * problem. These neighbors will be used afterwards
 * for computing areas.
 *
 * @param board_height      The height of the board.
 *
 * @param board_width       The width of the board.
 *
 * @param num_cells         The number of cells in the absolute
 *                          representation of the board.
 *
 * @param abs_to_rel_table  Transformation table from absolute
 *                          positions to relative ones.
 *
 * @param rel_to_abs_table  Transformation table from relative
 *                          positions to absolute ones.
 *
 * @param neighbors         This pointer will end up pointing
 *                          to the neighbors matrix.
 *
 * @param num_neighbors     This pointer will end up pointing
 *                          to an array that contains the number
 *                          of neighboring cells for each cell
 *                          in the absolute representation of the
 *                          board.
 */
void precompute_neighbors(
                    int board_height, int board_width, int num_cells,
                    int *abs_to_rel_table, int *rel_to_abs_table,
                    int (*neighbors)[4], int *&num_neighbors
                );



#endif
