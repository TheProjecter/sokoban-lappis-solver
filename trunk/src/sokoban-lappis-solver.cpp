#include"sokoban-lappis-solver.h"


/**
 * This variable will contain the number of
 * bits that an integer have in the machine
 * where this code is runned
 */
int int_bits = sizeof(int)*8;

char* solve_sokoban(char *buffer, int buf_size){

    cout << "begin" << endl;   
 
    vector< string > board;

    //Read and get the initial board
    int board_width = read_board(buffer,buf_size,board);

    for(int i=0;i<board.size();i++)
        cout << board[i] << endl;

    //Precompute the board and initialize the
    //tables
    int *abs_to_rel_table, *rel_to_abs_table, *goals_pos;
    soko_node *init_node;

    int num_cells = precompute_board(
                        board_width, board, abs_to_rel_table,
                        rel_to_abs_table, goals_pos,
                        init_node);

    //Save the number of cells for every node
    //and the number of ints needed for each bitmap
    soko_node::num_cells = num_cells;
    soko_node::arr_size = num_cells/int_bits;
    if(num_cells%int_bits!=0) soko_node::arr_size++;

    //Get the height of the board
    int board_height = board.size();


    //Precompute the neighbors matrix 
    int (*neighbors)[4], *num_neighbors;
    neighbors = new int[num_cells][4];
    
    precompute_neighbors(
                        board_height, board_width, num_cells,
                        abs_to_rel_table, rel_to_abs_table,
                        neighbors, num_neighbors
                    );

    //Make the auxiliary stack and compute the initial area
    int *stack_arr = new int[num_cells];
    //Let the soko_node class have the array as a static variable
    soko_node :: stack_arr = stack_arr;

    init_node->compute_area(   neighbors,
                    num_neighbors );

    vector< soko_node* > *v = init_node->get_sons(board_width,
                                        abs_to_rel_table, rel_to_abs_table,
                                        neighbors, num_neighbors);

    soko_node* sol_node = breadth_first_search(init_node,board_width,
                                                abs_to_rel_table,
                                                rel_to_abs_table,neighbors,
                                                num_neighbors,goals_pos);

    soko_node* curr_node;
    for(curr_node=sol_node;curr_node!=NULL;curr_node = curr_node->father)
        curr_node->print(board_height,board_width, abs_to_rel_table);

    char *solo=search_path(sol_node,board.size()*board_width,board_width,
                            abs_to_rel_table,rel_to_abs_table);
    /*
    char *solo = new char[6];
    solo[0]='U';
    solo[1]='\0';
    */
    return solo;
}

int read_board( char* buffer, int buf_size, vector< string > &board ){
    board.clear();
    int n = 0;
    int m = 0;
    int k = 0;

    while(k<buf_size){
        string s = "";
        while(buffer[k]!='\n' && buffer[k]!='\0')
            s += buffer[k++];
        
        board.push_back(s);
        n = max(n,(int)s.length());
        k++;
    }

    return n;
}


int precompute_board(int board_width, vector< string > &board,
		     int *&abs_to_rel_table, int *&rel_to_abs_table,
             int *&goals_pos, soko_node *&init_node ) {

    int board_size = board.size() * board_width;
    int lists_size = board_size/int_bits;
    if (board_size%int_bits != 0) lists_size++;
    int guy_x, guy_y, num_cell=0;
    const int moves[4][2] = {{1, 0},{0, 1},{-1, 0},{0,-1}};
    char c;

    //allocate memory for boards
    abs_to_rel_table = (int*) malloc(sizeof(int) * board_size);
    rel_to_abs_table = (int*) malloc(sizeof(int)* board_size);
    goals_pos = (int*) malloc(lists_size*sizeof(int));
    init_node = new soko_node();
    init_node->box_pos = (int*) malloc(lists_size*sizeof(int));

    //make the String board square
    //and find the guy
    for(int i=0;i<board.size();i++){
        board[i].resize(board_width, '#');
        //interprete every character of the line
        for(int j=0; j<board_width; j++){
            c = board[i][j];
            if ((c == GUYCHAR) || (c == GUYGOALCHAR)){
                guy_x = j;
                guy_y = i;
            }
        }
    }

    //fill in the board with UNKNOWN value
    memset(abs_to_rel_table, -1, board_size*sizeof(int));
    memset(rel_to_abs_table, 0, board_size*sizeof(int));
    memset(goals_pos, 0, lists_size*sizeof(int));
    memset(init_node->box_pos, 0, lists_size*sizeof(int));

    //use DFS to fill in the board
    dfs(board, abs_to_rel_table,
            rel_to_abs_table, goals_pos, init_node->box_pos,
            moves, board_width, guy_x, guy_y, num_cell);

    //For the initial state, the relative cell number 0
    //and dir_push is '\0' character
    //is the first valid position (it's always where the player starts)
    init_node->last_pos = 0;
    init_node->push_dir = '\0';
    
    //display the board
    cout << "ABSOLUTE BOARD :"<< endl;
    for(int i=0; i<board_width*board.size();i++){
        if (i%board_width == 0) cout << endl;
        switch (abs_to_rel_table[i]){
            case -1:
                cout<< " " << WALLCHAR;
                break;
            default:
                cout<<" " << (abs_to_rel_table[i]);
        }
    }

    //other boards
    cout << endl << "goals: ";
    for(int j=0; j<lists_size; j++){
        int i=0;
        int mask = 1;
        while(i<int_bits){
            if (mask & goals_pos[j]) cout << j*int_bits + i << " ";
            mask <<= 1;
            i++;
        }
    }

    cout << endl << "boxes : ";
    for(int j=0; j<lists_size; j++){
        int i=0;
        int mask = 1;
        while(i<int_bits){
            if (mask & (init_node->box_pos[j])) cout << j*int_bits + i << " ";
            mask <<= 1;
            i++;
        }
    }

    cout << endl << "rel_to_abs : ";
    for(int j=0; j<num_cell; j++){
        int abs_value = rel_to_abs_table[j];
        int y = abs_value/board_width;
        int x = abs_value%board_width;
        cout << "cell: " << j << " y: " << y << " x: " << x << endl;
    }

    //cout << endl << "num_cell: " << num_cell << endl;
    //cout << "Kevv part end here" << endl << endl;
    return num_cell;
}

void dfs(vector< string > &board, int *abs_to_rel_table, 
	 int *rel_to_abs_table, int *goals_pos, int *box_pos,
     const int moves[4][2], int board_width, int x, int y, int &c){   

    int nx, ny;

    //already visited cell
    if (abs_to_rel_table[x+y*board_width] != -1)
        return;

    switch(board[y][x]){
        case WALLCHAR:
            return;
        case GUYGOALCHAR:
            add_to_list(goals_pos, c);
            break;
        case GOALCHAR:
            add_to_list(goals_pos, c);
            break;
        case BOXGOALCHAR:
            add_to_list(goals_pos, c);
            add_to_list(box_pos, c);
            break;
        case BOXCHAR:
            add_to_list(box_pos, c);
            break;
    }

    //add values to translation tables
    abs_to_rel_table[x+y*board_width] = c;
    rel_to_abs_table[c] = x+y*board_width;
    c++;
    for(int i=0; i<4;i++){
        nx = x+moves[i][0];
        ny = y+moves[i][1];
        if(0<=nx && nx < board_width &&
           0<=ny && ny < board.size() &&
           board[ny][nx] != WALLCHAR)

            dfs(board, abs_to_rel_table,
                rel_to_abs_table, goals_pos, box_pos,
                moves, board_width, nx, ny, c);
    }
}

inline void add_to_list(int* liste, int index){
    int val;
    int u = index / int_bits;
    int w = index % int_bits;
    liste[u] |= 1<<w;
}

void precompute_neighbors(
                    int board_height, int board_width, int num_cells,
                    int *abs_to_rel_table, int *rel_to_abs_table,
                    int (*neighbors)[4], int *&num_neighbors
                ){

    //Create the arrays

    num_neighbors = new int[num_cells];
    memset(num_neighbors, 0, sizeof(int) * num_cells);

    const int move[4][2] = {{1,0},{0,1},{-1,0},{0,-1}};

    //For each cell in the relative representation,
    //look for its neighbors in the absolute one
    for(int i=0; i<num_cells; i++){
        int abs_pos = rel_to_abs_table[i];
        int y = abs_pos/board_width;
        int x = abs_pos%board_width;

        for(int j=0;j<4;j++){
            int ny = y + move[j][0];
            int nx = x + move[j][1];

            if( 0<=ny && ny<board_height &&
                0<=nx && nx<board_width &&
                abs_to_rel_table[ny*board_width + nx] != -1
                ){
                neighbors[i][ num_neighbors[i]++ ] =
                    abs_to_rel_table[ny*board_width + nx];
            }
        }
    }

    //cout << "neighbors" << endl;
    //for(int i=0;i<num_cells;i++){
    //    cout << i << ": " ;
    //    for(int j=0;j<num_neighbors[i];j++)
    //        cout << neighbors[i][j] << " ";
    //    cout << endl;
    //}

}

soko_node* breadth_first_search(soko_node *init_node, int board_width,
                                int* abs_to_rel_table, int *rel_to_abs_table,
                                int (*neighbors)[4], int *num_neighbors,
                                int *goals_pos) {
    soko_node* curr_node;
    vector< soko_node* > *sons;
    queue< soko_node* > fifo;
    fifo.push(init_node);

    while(!fifo.empty()){
        curr_node=fifo.front();
        fifo.pop();
        
        sons=curr_node->get_sons(board_width, abs_to_rel_table,
                                  rel_to_abs_table, neighbors, num_neighbors);

        for( int i=0; i<sons->size(); i++) {
            if( (*sons)[i]->is_solution(goals_pos) )
                return (*sons)[i];
            fifo.push((*sons)[i]);
        }
    }
    return NULL;
}

char* search_path(soko_node *curr_node, int board_size, int board_width,
                            int *abs_to_rel_table, int *rel_to_abs_table) {
    char *path;
    if(curr_node->father==NULL) {
        path=(char*)malloc(64*2048*sizeof(char));
        path[0]='\0';
        return path;
    }
    path=search_path(curr_node->father,board_size,board_width,
                                abs_to_rel_table,rel_to_abs_table);

    //calculates origin and destination positions
    int from=curr_node->father->last_pos;
    int abs_neigh_to=rel_to_abs_table[curr_node->last_pos];
    int ant_y=abs_neigh_to/board_width;
    int ant_x=abs_neigh_to%board_width;
    switch(curr_node->push_dir){
        case 'U':
            ant_y++;
            break;
        case 'D':
            ant_y--;
            break;
        case 'L':
            ant_x++;
            break;
        case 'R':
            ant_x--;
            break;
    }
    int to=abs_to_rel_table[ant_y*board_width + ant_x];
    
    int from_1=from/int_bits;
    int from_2=from%int_bits;
    int to_1=to/int_bits;
    int to_2=to%int_bits;

    int lists_size = board_size/int_bits;
    if (board_size%int_bits != 0) lists_size++;

    //bitmaps used to create the shortest path
    int *visited = (int*) malloc(lists_size*sizeof(int));
    int *move_u = (int*) malloc(lists_size*sizeof(int));
    int *move_d = (int*) malloc(lists_size*sizeof(int));
    int *move_l = (int*) malloc(lists_size*sizeof(int));
    int *move_r = (int*) malloc(lists_size*sizeof(int));

    memset(visited, 0, lists_size*sizeof(int));
    memset(move_u, 0, lists_size*sizeof(int));
    memset(move_d, 0, lists_size*sizeof(int));
    memset(move_l, 0, lists_size*sizeof(int));
    memset(move_r, 0, lists_size*sizeof(int));
    visited[to_1] |= 1<<to_2;

    cout << "from: " << from << endl;
    cout << "to: " << to << endl;
    for(int i=0;i<lists_size;i++) {
        cout << "visited " << i << ": " << visited[i] << endl;
        cout << "move_u " << i << ": " << move_u[i] << endl;
        cout << "move_d " << i << ": " << move_d[i] << endl;
        cout << "move_l " << i << ": " << move_l[i] << endl;
        cout << "move_r " << i << ": " << move_r[i] << endl;
    }

    int abs_last_pos=rel_to_abs_table[curr_node->last_pos];
    int last_pos_y=abs_last_pos/board_width;
    int last_pos_x=abs_last_pos%board_width;
    
    switch(curr_node->push_dir){
        case 'U':
            cout << "UP" << endl;
            last_pos_y--;
            break;
        case 'D':
            cout << "DOWN" << endl;
            last_pos_y++;
            break;
        case 'L':
            cout << "LEFT" << endl;
            last_pos_x--;
            break;
        case 'R':
            cout << "RIGHT" << endl;
            last_pos_x++;
            break;
    }
    abs_last_pos=last_pos_y*board_width+last_pos_x;
    int rel_ignore_box=abs_to_rel_table[abs_last_pos];

    while( ( visited[ from_1 ] & 1<<from_2 ) == 0 ){
        for(int i=0;i<lists_size;i++) {
            cout << "visited " << i << ": " << visited[i] << endl;
            cout << "move_u " << i << ": " << move_u[i] << endl;
            cout << "move_d " << i << ": " << move_d[i] << endl;
            cout << "move_l " << i << ": " << move_l[i] << endl;
            cout << "move_r " << i << ": " << move_r[i] << endl;
            cout << "from:" << (visited[ from_1 ] & 1<<from_2) << endl;
        }

        for(int p1 = 0; p1 < soko_node::arr_size; p1++){
            for(int p2 = 0; p2 < int_bits; p2++ ){
                if( visited[p1] & 1<<p2 ) {
                    // check for all neighbors which are in the area
                    // but not in boxes nor visited.
                    // for each of these set a 1 on the right bit of
                    // the u l r d bitmaps.
                    int rel_p=p1*int_bits+p2;
                    int abs_p=rel_to_abs_table[rel_p];
                    int p_y=abs_p/board_width;
                    int p_x=abs_p%board_width;
                    
                    int n_y=p_y+1;
                    int n_x=p_x;
                    int abs_n=n_y*board_width+n_x;
                    int rel_n=abs_to_rel_table[abs_n];
                    int n1=rel_n/int_bits;
                    int n2=rel_n%int_bits;
                    if((visited[n1] & 1<<n2)==0 &&
            (curr_node->father->area[n1] & ~curr_node->father->box_pos[n1] & 1<<n2)
                      && rel_n != curr_node->last_pos)
                        move_u[n1]|=1<<n2;

                    n_y=p_y-1;
                    n_x=p_x;
                    abs_n=n_y*board_width+n_x;
                    rel_n=abs_to_rel_table[abs_n];
                    n1=rel_n/int_bits;
                    n2=rel_n%int_bits;
                    if((visited[n1] & 1<<n2)==0 &&
            (curr_node->father->area[n1] & ~curr_node->father->box_pos[n1] & 1<<n2)
                      && rel_n != curr_node->last_pos)
                        move_d[n1]|=1<<n2;

                    n_y=p_y;
                    n_x=p_x+1;
                    abs_n=n_y*board_width+n_x;
                    rel_n=abs_to_rel_table[abs_n];
                    n1=rel_n/int_bits;
                    n2=rel_n%int_bits;
                    if((visited[n1] & 1<<n2)==0 &&
            (curr_node->father->area[n1] & ~curr_node->father->box_pos[n1] & 1<<n2)
                      && rel_n != curr_node->last_pos)
                        move_l[n1]|=1<<n2;

                    n_y=p_y;
                    n_x=p_x-1;
                    abs_n=n_y*board_width+n_x;
                    rel_n=abs_to_rel_table[abs_n];
                    n1=rel_n/int_bits;
                    n2=rel_n%int_bits;
                    if((visited[n1] & 1<<n2)==0 &&
            (curr_node->father->area[n1] & ~curr_node->father->box_pos[n1] & 1<<n2)
                      && rel_n != curr_node->last_pos)
                        move_r[n1]|=1<<n2;
                }
            }
        // update visited as bitwise-or of move_u-d-l-r
        // also, setting of to bit to 1
        for(int p1=0;p1<soko_node::arr_size;p1++)
            visited[p1]=move_u[p1]|move_d[p1]|move_l[p1]|move_r[p1];
        visited[to_1] |= 1<<to_2;
        }
    }
    
    cout << "path Found!! Startig Recovery:" << endl;
        for(int i=0;i<lists_size;i++) {
            cout << "visited " << i << ": " << visited[i] << endl;
            cout << "move_u " << i << ": " << move_u[i] << endl;
            cout << "move_d " << i << ": " << move_d[i] << endl;
            cout << "move_l " << i << ": " << move_l[i] << endl;
            cout << "move_r " << i << ": " << move_r[i] << endl;
            cout << "from:" << (visited[ from_1 ] & 1<<from_2) << endl;
        }

// path found, decode string
    int l=strlen(path);
    int pos_1=from_1;
    int pos_2=from_2;
    
    while(pos_1 != to_1 || pos_2!=to_2) {
        int rel_pos=pos_1*int_bits+pos_2;
        int abs_pos=rel_to_abs_table[rel_pos];
        int apos_y=abs_pos/board_width;
        int apos_x=abs_pos%board_width;
        
        if(move_u[pos_1] & 1<<pos_2) {
            path[l++]='U';
            apos_y--;
        }
        else if(move_d[pos_1] & 1<<pos_2) {
            path[l++]='D';
            apos_y++;
        }
        else if(move_l[pos_1] & 1<<pos_2) {
            path[l++]='L';
            apos_x--;
        }
        else{
            path[l++]='R';
            apos_x++;
        }
        path[l++]=' ';
        abs_pos=apos_y*board_width+apos_x;
        rel_pos=abs_to_rel_table[abs_pos];
        pos_1=rel_pos/int_bits;
        pos_2=rel_pos%int_bits;
    }
    path[l++]=curr_node->push_dir;
    path[l++]=' ';
    path[l++]='\0';

    free(visited);
    free(move_u);
    free(move_d);
    free(move_l);
    free(move_r);

    cout << "OK" << endl;

    return path;
}
