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

    return min_dist_matrix;
}

int* simple_bfs( int num_cells,
                      int *rel_to_abs_table,
                      int *abs_to_rel_table,
                      int board_width
        ){
    //Make the graph taking in consideration only
    //the places where a box can be pushed
    vector< vector<int> > graph = 
            vector< vector<int> >(num_cells,vector<int>());

    const int mov[2][2] = { {0,1}, {1,0}  }; //Horizontal and vertical
    //Check for each cell check both directions
    for(int i=0;i<num_cells;i++){
        int i_abs = rel_to_abs_table[i];
        int i_y = i_abs/board_width;
        int i_x = i_abs%board_width;
        for(int m=0;m<2;m++){
            //Take both neighbors
            int pn_y = i_y + mov[m][0];
            int pn_x = i_x + mov[m][1];
            int nn_y = i_y - mov[m][0];
            int nn_x = i_x - mov[m][1];
            //Check if both neighbors are alright
            int indx_pn = pn_y * board_width + pn_x;
            int indx_nn = nn_y * board_width + nn_x;

            if( abs_to_rel_table[ indx_pn ] != -1 &&
                abs_to_rel_table[ indx_nn ] != -1 ) {
                //Add the archs
                int pn_rel = abs_to_rel_table[indx_pn];
                int nn_rel = abs_to_rel_table[indx_nn];
                graph[i].push_back(pn_rel);
                graph[i].push_back(nn_rel);
            }
        }
    }
    
    //for(int i=0;i<num_cells;i++){
    //    cout << i << ": ";
    //    for(int j=0;j<graph[i].size();j++)
    //        cout << graph[i][j] << " ";
    //    cout << endl;
    //}


    //With the generated graph, make a bfs algorithm
    //for each cell and fill the min_dist_matrix
    int *min_dist_matrix = new int[num_cells*num_cells];

    //INF marks an unvisited node
    for(int i=0;i<num_cells;i++)
    for(int j=0;j<num_cells;j++)
        min_dist_matrix[i*num_cells + j] = INF + i*num_cells + j;

    for(int i=0; i<num_cells; i++){
        queue<int> q;
        
        //Precompute the offset of this row
        int row_offs = i*num_cells;
         
        //Push the first node and mark it    
        q.push(i);
        min_dist_matrix[row_offs + i] = 0;

        while(!q.empty()){
            int curr_cell = q.front();
            q.pop();

            int depth = min_dist_matrix[row_offs + curr_cell];

            //Check for unvisited sons and push them in the queue
            for( int s=0; s<graph[curr_cell].size() ; s++){
                int my_son = graph[curr_cell][s];
                if( min_dist_matrix[row_offs +  my_son] >= INF){
                    min_dist_matrix[row_offs +  my_son] = depth + 1;
                    q.push(my_son);
                }
            }
        }
    }

    return min_dist_matrix;
}


//.......................................................
//.......................................................
//.......................................................

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

int hungarian( int num_boxes, int num_goals,
                    int *box_goal_distance){
    hungarian_t prob;
    hungarian_init(&prob,box_goal_distance,num_boxes,num_goals,HUNGARIAN_MIN);


    //hungarian_print_rating(&prob);
    hungarian_solve(&prob);

    //hungarian_print_assignment(&prob);

    int sol = hungarian_benefit(&prob);
    //cout << "sol: " << sol << endl;
    hungarian_fini(&prob);
    return sol;
}

