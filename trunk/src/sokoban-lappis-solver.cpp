#include"sokoban-lappis-solver.h"


int int_bits = sizeof(int)*8;

void solve_sokoban(char *buffer, int buf_size){

    cout << "begin" << endl;   
 
    vector< string > board;

    //Read and get the initial board

    int board_width = read_board(buffer,buf_size,board);

    for(int i=0;i<board.size();i++)
        cout << board[i] << endl;

    int *abs_to_rel_table, *rel_to_abs_table, *goals_pos;

    soko_node *init_node;

    int num_cells = precompute_board(
                        board_width, board, abs_to_rel_table,
                        rel_to_abs_table, goals_pos,
                        init_node);

    int board_height = board.size();

    //Precompute the neighbors matrix
    
    int (*neighbors)[4], *num_neighbors;
    neighbors = new int[num_cells][4];
    
    precompute_neighbors(
                        board_height, board_width, num_cells,
                        abs_to_rel_table, rel_to_abs_table,
                        neighbors, num_neighbors
                    );

    //Make the auxiliary stack
    int *stack_arr = new int[num_cells];

    compute_area(   num_cells, neighbors,
                    num_neighbors, stack_arr, init_node );

    init_node->print( board_height,board_width, num_cells,
                      abs_to_rel_table);

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
    init_node->area = (int*) malloc(lists_size*sizeof(int));
    init_node->box_pos = (int*) malloc(lists_size*sizeof(int));
    
    //make the String board square
    //and find the guy
    for(int i=0;i<board.size();i++){
	board[i].resize(board_width, '#');
	//read a new line
	cout << board[i] << endl;
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
    memset(init_node->area, 0, lists_size*sizeof(int));
    memset(init_node->box_pos, 0, lists_size*sizeof(int));

    //use DFS to fill in the board
    dfs(board, abs_to_rel_table,
	rel_to_abs_table, goals_pos, init_node->box_pos,
	moves, board_width, guy_x, guy_y, num_cell);

    //add the guy position to his board
    add_to_list(init_node->area, abs_to_rel_table[guy_x+guy_y*board_width]);

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

//other boards
    cout << endl << "area (guy): ";
    for(int j=0; j<lists_size; j++){
	int i=0;
	int mask = 1;
	while(i<int_bits){
	    if (mask &(init_node->area[j])) cout << j*int_bits + i << " ";
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

    cout << endl << "num_cell: " << num_cell << endl;
    cout << "Kevv part end here" << endl << endl;
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
	dfs(board, abs_to_rel_table,
	    rel_to_abs_table, goals_pos, box_pos,
	    moves, board_width, nx, ny, c);
    }
}

inline void add_to_list(int* liste, int index){
    int val;
    int u = index / int_bits;
    val = (liste[u] | (1<<(index - int_bits*u)));
    liste[u] = val;
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

void compute_area(  int num_cells, 
                    int (*neighbors)[4], int *num_neighbors,
                    int *stack_arr, soko_node *node ){

    int arr_size = num_cells/int_bits;
    if( num_cells%int_bits !=0 ) arr_size++;

    //find a valid position in the node to start with
    int p1 = 0;
    int p2 = 0;
    int mask = 1;
    while(true){
        while( p2<int_bits && !( node->area[p1] & mask ) ){
            p2++;
            mask<<=1;
        }
        if(p2<int_bits) break;
        p1++;
    }

    //code for a DFS using a stack...

    int st_sz = 0; //The stack's size

    //push the first element
    stack_arr[st_sz++]= p1*int_bits + p2;

    while( st_sz != 0 ){

        //Take the top element and pop
        int current_cell = stack_arr[ --st_sz ];

        //Check neighbors
        for(int i=0; i<num_neighbors[current_cell]; i++){

            int neig_cell = neighbors[current_cell][i];
            p1 = neig_cell/int_bits;
            p2 = neig_cell%int_bits;
            mask = 1<<p2;

            if( !( node->area[p1]&mask ) ){
                node->area[p1] |= mask;

                //push the element if there's not a box
                //on it
                if( !( node->box_pos[p1]&mask ) )
                    stack_arr[st_sz++] = neig_cell;
            }
        }
    }
}


