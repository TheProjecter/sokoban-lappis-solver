#ifndef _SLS_SOKO_NODE_H
#define _SLS_SOKO_NODE_H

#include<iostream>
#include<vector>

using namespace std;

class soko_node{
  public:
    /**
     * The area where the player can move
     */
    int *area;
    /**
     * The position of the boxes in this node
     */
    int *box_pos;
    /**
     * A pointer to the father of this node,
     * if set to NULL, then this is a root
     * node
     */
    soko_node *father;
    
    /**
     * Constructor of the class
     * @param father    A pointer to the father of the
     *                  node, NULL if is a root node
     */
    soko_node(soko_node *father=NULL);

    /**
     * Print the board using the next code:
     * '#' = Unaccessible place
     * '.' = Accessible place in the area
     * ' ' = Accesible place
     * '$' = Box
     * 'x' = Box inside the area
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
     */
    void print( int board_height, int board_width,
                int num_cells, int *abs_to_rel_table);

    /**
     * Given a node, makes a flood algorithm that calculates
     * the active area that can be reached by the player.
     *
     * @param num_cells         The number of total reachable cells
     *                          in the board.
     *
     * @param neighbors         A matrix with the neighboring cells
     *                          of every cell in the relative representation
     *
     * @param num_neighbors     An array with the number of neighboring
     *                          cells of each cell in the relative representation
     *
     * @param stack_arr         An array of size "num_cells" to be used
     *                          as a stack.
     *
     * @param node              The node where to make the flood.
     */
    void compute_area(  int num_cells, int (*neighbors)[4],
                        int *num_neighbors, int *stack_arr );
    

    vector< soko_node > get_sons();
};

#endif
