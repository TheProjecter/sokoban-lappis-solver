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
#include "soko_algorithm.h"


soko_node* breadth_first_search(soko_node *init_node, int board_height,
                                int board_width,
                                int* abs_to_rel_table, int *rel_to_abs_table,
                                int* deadlock_list, int num_cells,
                                int (*neighbors)[4], int *num_neighbors,
                                int *goals_pos) {
    soko_node* sol_node = NULL;
    soko_node* curr_node;
    vector< soko_node* > *sons;
    queue< soko_node* > fifo;
    fifo.push(init_node);
    int generated_nodes = 1; //All the nodes created
    int expanded_nodes = 0;
    int queued_nodes = 1; //All the nodes pushed in the queue
                          //(those that weren't in deadlock)

    while(!fifo.empty() && sol_node == NULL){

        expanded_nodes++;
        curr_node=fifo.front();
        fifo.pop();

        sons=curr_node->get_sons(board_width, abs_to_rel_table,
                                 rel_to_abs_table, neighbors, num_neighbors);
        generated_nodes += sons->size();

        for( int i=0; i<sons->size(); i++) {
            bool qwe = *curr_node < *((*sons)[i]);
            if( (*sons)[i]->is_solution(goals_pos)){
                sol_node = (*sons)[i];
                break;
            }
            if (is_deadlock(rel_to_abs_table, abs_to_rel_table, 
                            deadlock_list, neighbors,
                            num_neighbors, (*sons)[i], num_cells,
                            board_height, board_width, goals_pos))
                continue;
            queued_nodes++;
            fifo.push((*sons)[i]);
        }
        delete sons;
    }

    cout << "BFS Results" << endl << ".............." << endl
         << "Generated nodes: " << generated_nodes << endl
         << "Queued nodes: " << queued_nodes << endl
         << "Expanded nodes: " << expanded_nodes << endl
         << "Sol found: " << (sol_node == NULL? "no":"yes") << endl <<endl;

    return sol_node;
}
