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
#include "hash_table.h"

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

soko_node* a_star_search(soko_node *init_node, int board_height,
                                int board_width,
                                int* abs_to_rel_table, int *rel_to_abs_table,
                                int* deadlock_list, int num_cells,
                                int (*neighbors)[4], int *num_neighbors,
                                int *goals_pos, int *goals_rel_pos, int num_boxes,
                                int num_goals) {

    hash_table *ht=new hash_table();
    
    //Precompute the min_dist_matrix to make possible the
    //calculations of heuristics and build the box_goal_distance
    //matrix to make it available for further use
    int *min_dist_matrix =
        pusher_relative_bfs( num_cells,
                          rel_to_abs_table,
                          abs_to_rel_table,
                          board_width,
                          neighbors, num_neighbors
            );

    //int *min_dist_matrix =
    //    simple_bfs( num_cells,
    //                      rel_to_abs_table,
    //                      abs_to_rel_table,
    //                      board_width
    //        );

    int *box_goal_distance = new int[ num_boxes*num_goals ];

    //cout << endl << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
    //for(int i=0; i<num_cells;i++){
    //    for(int j=0; j<num_cells; j++){
    //        int min_dist = min_dist_matrix[i*num_cells+j];
    //        printf("%5d ",min_dist);

    //    }
    //    cout << endl;
    //}
    //cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

    //The node that will have the solution and an iterator
    soko_node* sol_node = NULL;
    soko_node* curr_node;
    //A vector to kep track of the sons of a vector
    vector< soko_node* > *sons;
    //The queue with the comparisson class
    priority_queue< soko_node*, vector< soko_node* >, soko_comp > p_queue;

    int generated_nodes = 1; //All the nodes created
    int expanded_nodes = 0;
    int queued_nodes = 1; //All the nodes pushed in the queue
                          //(those that weren't in deadlock)

    //Calculate the heuristic of the first node
    init_node->calc_heur( num_boxes, num_goals, goals_rel_pos, 
                        min_dist_matrix, box_goal_distance );


    init_node->print( board_height, board_width, abs_to_rel_table );

    //Push the first node and begin algorithm
    p_queue.push(init_node);

    while(!p_queue.empty() && sol_node == NULL){

        expanded_nodes++;
        curr_node = p_queue.top();
        p_queue.pop();

        sons=curr_node->get_sons(board_width, abs_to_rel_table,
                                rel_to_abs_table, neighbors, num_neighbors);
        generated_nodes += sons->size();

        //cout << generated_nodes << endl;
        if(expanded_nodes%10000 == 0){
            curr_node->print( board_height, board_width, abs_to_rel_table );
            cout << "Exp nodes: " << expanded_nodes << endl;
        }

        for( int i=0; i<sons->size(); i++) {

            soko_node *my_son = (*sons)[i];

            if( my_son->is_solution(goals_pos)){
                sol_node = (*sons)[i];
                break;
            }
            if (is_deadlock(rel_to_abs_table, abs_to_rel_table, 
                            deadlock_list, neighbors,
                            num_neighbors, my_son, num_cells,
                            board_height, board_width, goals_pos))
                continue;

            if(!ht->searchNode(my_son)) {
                queued_nodes++;
        
                //Calculate the heuristic of the node
                my_son->calc_heur( num_boxes, num_goals, goals_rel_pos, 
                                    min_dist_matrix, box_goal_distance );

                p_queue.push(my_son);
            }
        }
        delete sons;
    }

    ht->statistics();

    cout << "A* Results" << endl << ".............." << endl
         << "Generated nodes: " << generated_nodes << endl
         << "Queued nodes: " << queued_nodes << endl
         << "Expanded nodes: " << expanded_nodes << endl
         << "Sol found: " << (sol_node == NULL? "no":"yes") << endl <<endl;

    return sol_node;
}
