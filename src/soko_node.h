#ifndef _SLS_SOKO_NODE_H
#define _SLS_SOKO_NODE_H

#include<iostream>
#include<vector>
#include<string.h>

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
     * This integer will hold the relative position
     * of the box that it's father pushed. (not
     * the new position of the box, but the one
     * before pushing it)
     */
    int last_pos;
    /**
     * The number of cells in the relative
     * representation of the board
     */
    static int num_cells;
    /**
     * A stack used for flooding algorithm
     */
    static int *stack_arr;
    /**
     * The number of integers in each bitmap
     */
    static int arr_size;

    /**
     * Constructors of the class
     * 
     * @param last_pos  Relative position of the box
     *                  that the father of this node
     *                  pushed.
     *
     * @param father    A pointer to the father of the
     *                  node, NULL if is a root node
     */
    soko_node(int last_pos, soko_node *father);

    //Empty constructor
    soko_node(){ this->father=NULL; }

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
     * @param abs_to_rel_table  Transformation table from absolute
     *                          positions to relative ones.
     */
    void print( int board_height, int board_width,
                int *abs_to_rel_table);

    /**
     * Makes a flood algorithm that calculates
     * the active area that can be reached by the player.
     * It makes the flood starting from the cell stored in
     * the atribute last_pos of this node. It will also
     * initialize the variable "area" of this node
     *
     * @param neighbors         A matrix with the neighboring cells
     *                          of every cell in the relative representation
     *
     * @param num_neighbors     An array with the number of neighboring
     *                          cells of each cell in the relative
     *                          representation
     */
    void compute_area(  int (*neighbors)[4], int *num_neighbors );
    
    /**
     * Calculate the sons of a node and return them in a vector
     *
     * @param board_width       The width of the board.
     *
     * @param abs_to_rel_table  Transformation table from absolute
     *                          positions to relative ones.
     *
     * @param rel_to_abs_table  Transformation table from relative
     *                          positions to absolute ones.
     *
     * @param neighbors         A matrix with the neighboring cells
     *                          of every cell in the relative representation
     *
     * @param num_neighbors     An array with the number of neighboring
     *                          cells of each cell in the relative
     *                          representation
     *
     * @return                  The sons of the node
     */
    vector< soko_node* > *get_sons(int board_width, int* abs_to_rel_table,
                                int *rel_to_abs_table,
                                int (*neighbors)[4], int *num_neighbors);

    /**
     * Determines if the soko_node calling this method is a terminal node or not.
     * i.e. if all boxes are in goal position.
     *
     * 
     * @return                  True if node is a terminal node. False otherwise
     */
    bool is_solution(int *goals_pos);

  private:
    /**
     * Given a box and a neighbor of the box (both in
     * relative positions) calculates the oposite neighbor
     * of the box.
     *
     * @param board_width       The width of the board.
     *
     * @param abs_to_rel_table  Transformation table from absolute
     *                          positions to relative ones.
     *
     * @param rel_to_abs_table  Transformation table from relative
     *                          positions to absolute ones.
     *
     * @param box               The relative position of the board
     *
     * @param neigh             The relative position of a neighbor
     *
     * @return                  The relative position of the oposite neighbor
     *                          of the box
     */
    inline static int find_oposite(int board_width, int* abs_to_rel_table,
                     int *rel_to_abs_table, int box, int neigh);
};



#endif
