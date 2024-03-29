/**
 * KTH - Royal Institute of Technology
 * DD2380 - Artificial Intelligence
 * Autumn 2010
 *
 * Heuristics used for the sokoban solver
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
#include<queue>
#include<vector>
#include<stack>
#include<utility>

//Maximum number of cells
#define INF 5000
#define N 55
#define min(a,b) (a<b?a:b)

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

/**
 * Calculates the minimum distance from every cell to
 * any other performing a BFS.
 *
 * @param num_cells         The number of cells in the rel representation
 *                          of the board
 *
 * @param rel_to_abs_table  Transformation table from relative
 *                          positions to absolute ones.
 *
 * @param abs_to_rel_table  Transformation table from absolute
 *                          positions to relative ones.
 *
 * @param board_width       The width of the board
 * 
 * @return                  A "num_cells x num_cells" matrix, represented
 *                          as an array, where the position [i][j] contains
 *                          the min distance between the cells i and j
 *                          (according to the BFS)
 */
int*   simple_bfs( int num_cells,
                      int *rel_to_abs_table,
                      int *abs_to_rel_table,
                      int board_width
        );

/**
 * Calculates the minimum distance from every cell to
 * any other performing a BFS. The difference of this algorithm
 * is the graph where to perform the BFS: a more complex graph
 * where the position of the player is taking in account. This
 * technique actually finds the real cost heuristic h*(n)
 * if there where only one box on the board.
 * 
 * @param num_cells         The number of cells in the rel representation
 *                          of the board
 *
 * @param rel_to_abs_table  Transformation table from relative
 *                          positions to absolute ones.
 *
 * @param abs_to_rel_table  Transformation table from absolute
 *                          positions to relative ones.
 *
 * @param board_width       The width of the board
 *
 * @param neighbors         A matrix with the neighboring cells
 *                          of every cell in the relative representation
 *
 * @param num_neighbors     An array with the number of neighboring
 *                          cells of each cell in the relative
 *                          representation
 * 
 * @return                  A "num_cells x num_cells" matrix, represented
 *                          as an array, where the position [i][j] contains
 *                          the actual number of pushes needed to take a box
 *                          from cell "i" to cell "j"
 */
int*   pusher_relative_bfs( int num_cells,
                      int *rel_to_abs_table,
                      int *abs_to_rel_table,
                      int board_width,
                      int (*neighbors)[4], int *num_neighbors
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
                    int (*cost)[N]);



//............................................................................
//                          Others (auxiliary functions)
//............................................................................

/**
 * Structure of an edge used for calculating
 * biconex components
 */
struct edge{
    /**
     * The edge will be "v -> w", where
     * v & w are neighboring cells on the board.
     * W is the neighbor number "n" of v:
     *      neighbor[v][n] == w
     */
    int v, w, n;
    edge();
    edge(int v, int w, int n);

    //Function for making comparissons between
    //nodes
    bool operator==(const edge &e) const;
};

/**
 * Global values used on each iteration
 * for calculating biconex components
 */
struct cbc_global{
    //Stack of edges in current biconex component
    stack<edge> stk;
    //Number of visited nodes in DFS
    int count;
    //Number of found biconex components
    int number_of_bcn_comp;
    //Array of visited nodes, -1 == not visited.
    //If visit[i] != -1, then the node i was the
    //visit[i]th to be visited
    int *visit;
    //least[i] contains the number of the least
    //node where there is a returnin edge from
    //the ith node to it.
    int *least;
    //The graph where to perform analysis (undirected)
    int (*graph)[4];
    //The arity of each node
    int *graph_arity;
    //biconex_edge[i][j] is the number of the
    //biconex component where the edge "i -> graph[i][j]"
    //belongs to
    int (*biconex_edges)[4];
    //art_node[i] will be true if the cell i is an art point
    bool *art_node;
    //Number of sons of the origin generated in the DFS
    int sons_of_o;
};

/**
 * Algorithm for calculating the biconex components and
 * articulation points on the board
 *
 * @param num_cells         The number of cells on the board
 *
 * @param neighbors         A matrix with the neighboring cells
 *                          of every cell in the relative representation
 *
 * @param num_neighbors     An array with the number of neighboring
 *                          cells of each cell in the relative
 *                          representation
 *
 * @param biconex_edges     The algorithm will build this matrix, in such
 *                          way that:
 *                              biconex_edge[i][j] is the number of the
 *                              biconex component where the edge
 *                              "i -> neighbors[i][j]" belongs to.
 *
 * @return                  An array "art_node" where art_node[i]
 *                          is true iff the cell i is an articulation point
 *                          on the board.
 */
bool* calc_biconex_comp( int num_cells, int (*neighbors)[4],
                        int *num_neighbors, int (*biconex_edges)[4]);

/**
 * DFS recoursive function used for calculation of biconex components
 *
 * @param x     Node to be analized
 *
 * @param p     Node analyzed before this one. (The edge p -> x) was
 *              followed
 *
 * @param g_data    Global data for the biconex algorithm
 */
void biconex_calc_dfs(int x,int p, cbc_global &g_data);

//This algorithm builds the complex graph that takes into account
//the existance of the player to make possible the pushes.
vector< vector< pair<int,int> > >* calc_pr_graph(
        int num_cells, int *rel_to_abs_table,
        int *abs_to_rel_table, int board_width,
        bool* art_node, int (*biconex_edges)[4],
        int (*neighbors)[4]);


#endif

