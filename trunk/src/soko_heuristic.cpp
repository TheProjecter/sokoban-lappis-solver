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

int*   pusher_relative_bfs( int num_cells,
                      int *rel_to_abs_table,
                      int *abs_to_rel_table,
                      int board_width,
                      int (*neighbors)[4], int *num_neighbors
        ){

    //Calculate the articulation points and bi-conex
    //components on the board
    int (*biconex_edges)[4] = new int[num_cells][4];

    bool* art_node = calc_biconex_comp(num_cells, neighbors,
                                            num_neighbors, biconex_edges);
   
    //cout << ".....biconex comp....." << endl;
    //for(int i=0;i<num_cells;i++){
    //    cout << i << ": " << (art_node[i]?"articulation node!":"") << endl;
    //    for(int j = 0; j<num_neighbors[i]; j++)
    //        cout << "   -> "
    //             << neighbors[i][j] << " = " << biconex_edges[i][j] << endl;
    //}
    //cout << "......................" << endl;

    //Once having the articulation points, it's time to create the
    //graph where to do the BFS

    vector< vector< pair<int,int> > > *graph =
                calc_pr_graph(num_cells, rel_to_abs_table,
                                abs_to_rel_table, board_width, art_node,
                                biconex_edges, neighbors);

    //cout << "....complex graph!......." << endl;
    //for(int i=0;i<num_cells;i++){
    //    cout << i << endl;
    //    for(int d=0;d<4;d++){
    //        cout << " -> " << d << ": ";
    //        for(int j=0; j<graph[d][i].size() ; j++){
    //            cout << graph[d][i][j].first << "("
    //                << graph[d][i][j].second << "), ";
    //        }
    //        cout << endl;
    //    }
    //}

    //Now make a BFS in the complex_graph for each
    //cell and each possible initial position of
    //the player.
    //Fill the min_dist_matrix
    int *min_dist_matrix = new int[num_cells*num_cells];

    //Auxiliar matrix used for calculating the minimum
    //to each node. Then they will be traspased to the
    //min_dist_matrix
    int (*mdm_aux)[4] = new int[num_cells][4];

    //INF marks an unvisited node
    for(int i=0;i<num_cells;i++){
        for(int j=0;j<num_cells;j++){
            min_dist_matrix[i*num_cells + j] = 
                    INF + i*num_cells + j;
        }
    }


    //Make the BFS for each node
    for(int i=0; i<num_cells; i++){ //node

        //Mark all nodes as unvisited
        for(int j=0;j<num_cells;j++)
        for(int k=0;k<4;k++)
            mdm_aux[j][k] = INF + i*num_cells + j;
        
        queue< pair<int, int> > q;
         
        //Push the first node and mark it.
        //Take in consideration all four directions.
        for(int k=0;k<4;k++){
            q.push( make_pair(i,k) );
            mdm_aux[i][k] = 0;
        }

        while(!q.empty()){
            int curr_cell = q.front().first;
            int curr_dir = q.front().second;

            q.pop();

            int depth = mdm_aux[curr_cell][curr_dir];

            //Check for unvisited sons and push them in the queue
            for( int s=0; s<graph[curr_dir][curr_cell].size() ; s++){
                int my_son = graph[curr_dir][curr_cell][s].first;
                int my_new_dir = graph[curr_dir][curr_cell][s].second;

                if( mdm_aux[my_son][my_new_dir] >= INF){

                    mdm_aux[my_son][my_new_dir] = depth + 1;
                    q.push( make_pair(my_son, my_new_dir) );

                }
            }
        }

        //Now, place in min_dist_matrix the minimum of the
        //four possible positions to arrive to each node

        //Precompute the offset of this row
        int row_offs = i*num_cells;

        for(int j=0; j<num_cells; j++){

            int tot_offs = row_offs + j;

            for(int d=0; d<4; d++){
                min_dist_matrix[tot_offs] =
                    min( min_dist_matrix[tot_offs],
                         mdm_aux[j][d]);
            }
        }
    }

    delete[] mdm_aux;
    delete[] art_node;
    delete[] biconex_edges;

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

        int mini = INF;

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

//.......................................................
//.......................................................
//.......................................................

//Auxiliary class for calculating the biconex components
edge::edge(){}

edge::edge(int v, int w, int n){
    this->v=v;
    this->w=w;
    this->n=n;
}

bool edge::operator==(const edge &e) const{
    return this->v == e.v && this->w == e.w;
}


bool* calc_biconex_comp( int num_cells, int (*neighbors)[4],
                        int *num_neighbors, int (*biconex_edges)[4]){

    int *visit = new int[num_cells];
    int *least = new int[num_cells];
    bool *art_node = new bool[num_cells];
    memset(visit, -1, sizeof(int)*num_cells); 
    memset(art_node, false, sizeof(bool)*num_cells); 

    stack< edge > stk;

    cbc_global g_data = {stk,0,0,
                        visit,least,neighbors,num_neighbors,
                        biconex_edges, art_node, 0};

    biconex_calc_dfs(0,0,g_data);

    if(g_data.sons_of_o > 1)
        art_node[0] = true;

    delete[] visit;
    delete[] least;

    return art_node;
}

void biconex_calc_dfs(int x,int p, cbc_global &g_data){
    //Mark the node as visited
    g_data.least[x] = g_data.visit[x] = g_data.count++;
    //Calculate all the successors of the node different than its
    //parent
    for(int i=0; i < g_data.graph_arity[x] ;i++){

        int w = g_data.graph[x][i];
        if(w==p)
            continue;

        //If the son hasn't been visited or was visited
        //before 'x' (x -> w is a returning edge) add
        //the edge to the current biconex component
        if( g_data.visit[w] < g_data.visit[x] )
           g_data.stk.push( edge(x,w,i) );

        //If the node hasn't been visited
        if( g_data.visit[w] == -1 ){
            //If this is the origin, mark a new
            //son found
            if(x==0)
                g_data.sons_of_o++;

            biconex_calc_dfs(w,x, g_data);

            //Actualize minimum values, check if there was a "returning path"
            //from w to a parent of x
            g_data.least[x] = min(g_data.least[x], g_data.least[w]);

            if(g_data.visit[x] <= g_data.least[w]){
                //If there is not such returning path to a father
                //of x, then x is an articulation point. Dump
                //the stack. Mark all the nodes whitin the edges
                //to belong to a new biconex compound.
                edge me(x,w,i);

                while(! ( g_data.stk.top() == me ) ){
                    int a = g_data.stk.top().v;
                    int b = g_data.stk.top().w;
                    int n = g_data.stk.top().n;
                    g_data.stk.pop();

                    g_data.biconex_edges[a][n] = g_data.number_of_bcn_comp;
                    //find the oposite and mark it
                    for(int k=0;;k++)
                        if( g_data.graph[b][k] == a){
                            g_data.biconex_edges[b][k] = g_data.number_of_bcn_comp;
                            break;
                        }
                    
                }

                //Mark the biconex number of this edge (x->w) and
                //mark x as an articulation point of the graph
                g_data.stk.pop();
                g_data.biconex_edges[x][i] = g_data.number_of_bcn_comp;
                //find the oposite and mark it
                for(int k=0;;k++)
                    if( g_data.graph[w][k] == x){
                        g_data.biconex_edges[w][k] = g_data.number_of_bcn_comp;
                        break;
                    }

                if(x!=0)
                    g_data.art_node[x] = true;

                g_data.number_of_bcn_comp++;
            }
        }
        else{
            //If it's already visited, actualize least number
            g_data.least[x] = min(g_data.least[x], g_data.visit[w] );
        }

    }
}


vector< vector< pair<int,int> > >* calc_pr_graph(
        int num_cells, int *rel_to_abs_table,
        int *abs_to_rel_table, int board_width,
        bool *art_node, int (*biconex_edges)[4],
        int (*neighbors)[4]){

    //There will be four dimentions of the graph because each node
    //will be represented as four nodes, each one depending on the
    //last position of the player, and:
    //
    //0 = UP, 1 = RIGHT, 2 = DOWN, 3 = LEFT
    //
    //The matrix contains pairs of ints:
    //  - The first number is the number of the cell
    //  - The second is one of the four posible positions of the
    //  player following the same order stated above

    vector< vector< pair<int,int> > > *graph =  
        new vector< vector< pair<int,int> > >[4]();


    for(int i=0;i<4;i++)
        (graph)[i] = vector< vector< pair<int,int> > >
                    (num_cells, vector< pair<int,int> >());

    //For making calculations more simple, we will reduce all cases
    //to a comon one: The case where the pusher is in the cell UP
    //of the node, then we are going to try to see if the cell can be pushed
    //UP, RIGHT, DOWN or LEFT. This means that for each position of the
    //player, we will check first if the box can be pushed in the same
    //direction where the player is and then we check the other directions
    //clockwise.
    //The next matrix is used for making possible such transformations:
    //The dimentions are:
    //   Position_of_player x Direction_to_push x Y and X deltas.
    //
    //Remember that the direction to push will go clockwise starting from
    //the same place where the player is

    int trans[4][4][2];
    //We will fill the matrix iteratively, but filling by hand the
    //first case
    trans[0][0][0] = -1; trans[0][0][1] =  0; //UP UP
    trans[0][1][0] =  0; trans[0][1][1] =  1; //UP RIGHT
    trans[0][2][0] =  1; trans[0][2][1] =  0; //UP DOWN
    trans[0][3][0] =  0; trans[0][3][1] = -1; //UP LEFT

    //All the others are calculated iteratively
    for(int p=1; p<4; p++){ //player
    for(int d=0; d<4; d++){ //direction
    for(int c=0;c<2;c++){ //coordinate
        trans[p][d][c] =
            trans[p-1][ (d+1)%4 ][c];
    }}}

    //Build the graph for each node and each possible position
    for(int i=0;i<num_cells;i++){ //the node

        int i_abs = rel_to_abs_table[i];
        int i_y = i_abs/board_width;
        int i_x = i_abs%board_width;

        for(int p=0; p<4; p++){ //the position of the player
            //The first direction is always the cell where
            //the player is.
            int p_y = i_y + trans[p][0][0];
            int p_x = i_x + trans[p][0][1];
            int p_rel = abs_to_rel_table[ p_y*board_width + p_x ];

            //Check that the player is not a wall
            if(p_rel == -1) continue;

            for(int d=0; d<4; d++){ //The direction
                //Basic check: the box can be
                //pushed in that direction (the cell that is in the
                //same direction, and in the oposite, are not walls).

                int dp_y = i_y + trans[p][d][0]; 
                int dp_x = i_x + trans[p][d][1]; //direction: positive
                int dn_y = i_y - trans[p][d][0];
                int dn_x = i_x - trans[p][d][1]; //direction: negative

                int indx_dp = dp_y * board_width + dp_x;
                int indx_dn = dn_y * board_width + dn_x;

                int dest_rel = abs_to_rel_table[ indx_dp ];
                int op_dest_rel = abs_to_rel_table[ indx_dn ];
                //op_dest_rel is the position that the player
                //will have to take to push the box to dest_rel

                if(  dest_rel == -1 ||
                     op_dest_rel == -1 )
                        continue;

                //Advanced check: if the cell checked is
                //an articulation point, then it is necessary
                //to know if there is a path from the position of
                //the player to the position behind the box.
                if( art_node[i] ){
                    //It will only be necessary to check if
                    //the arch from the node to the player and
                    //the arch form the node to the op_dest_rel
                    //belong to the same biconex component. If
                    //this doesn't happen, then the push is not
                    //possible

                    int node_player, node_op;
                    bool np = false, no = false;

                    for(int k=0; !np || !no ;k++){
                        if( neighbors[i][k] == p_rel ){
                            node_player = biconex_edges[i][k];
                            np = true;
                        }

                        if( neighbors[i][k] == op_dest_rel ){
                            node_op = biconex_edges[i][k];
                            no = true;
                        }
                    }

                    if(node_player != node_op) continue;
                }

                //If everything is ok, then add the arch to the graph
                
                //Calculate the direction where the player will
                //be after pushing the box
                int dir_of_push = (p+d)%4;
                
                int player_new_pos = (dir_of_push + 2)%4; //Oposite of push
                
                graph[p][i].push_back( make_pair( dest_rel, player_new_pos) ); 

            }
        }
    }
    
    return graph;
}

