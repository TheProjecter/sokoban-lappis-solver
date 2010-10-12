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
#include"soko_heuristic.h"

int* manhattan_dist( int num_cells,
                     int *rel_to_abs_table,
                     int board_width 
                    ){
    //Make the matirx (num_cells x num_cells)
    int *min_dist_matrix = new int[num_cells*num_cells];
    memset(min_dist_matrix,0,sizeof min_dist_matrix);

    //For each cell, calculate the manhattan distance,
    //observe that only half of the matrix is traveled
    for(int i=0; i<num_cells; i++){

        int i_y = rel_to_abs_table[i]/board_width;
        int i_x = rel_to_abs_table[i]%board_width;

        for(int j=0;j<i;j++){

            int j_y = rel_to_abs_table[j]/board_width;
            int j_x = rel_to_abs_table[j]%board_width;

            int delta_x = i_x - j_x;
            int delta_y = i_y - j_y;

            if(delta_x < 0) delta_x = -delta_x;
            if(delta_y < 0) delta_y = -delta_y;

            min_dist_matrix[i*num_cells + j] = delta_x + delta_y;
            min_dist_matrix[j*num_cells + i] = delta_x + delta_y;
        }
    }

    //cout << endl << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
    //for(int i=0; i<num_cells;i++){
    //    for(int j=0; j<num_cells; j++)
    //        cout << min_dist_matrix[i*num_cells+j] << " ";
    //    cout << endl;
    //}
    //cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

    return min_dist_matrix;
}


int nearest_goal( int num_boxes, int num_goals,
                    int *box_goal_distance){
    int sol = 0;

    //For each box, calculate the minumum distance to a goal
    for( int i=0; i < num_boxes; i++){

        int mini = 99999; //Inf...

        for( int j=0; j < num_goals ; j++)
            mini = min(mini, box_goal_distance[i*num_goals+j]);
        sol += mini;
    }

    return sol;
}
