#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include<iostream>
using namespace std;

class Node{
  public:
    int *area;
    int *box_pos;
};


int read_board( vector< string > &board );

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
            int *abs_to_rel_table, int *rel_to_abs_table,
            int *goals_pos, Node *init_node
            );

//...........KEVIN FUNCTIONs............



//...........END KEVIN FUNCTIONs............


//...........CARLOS FUNCTIONs............



//...........END CARLOS FUNCTIONs............

