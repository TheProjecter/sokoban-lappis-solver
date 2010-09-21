#include"sokoban-lappis-solver.h"

int main(){
    
    vector< string > board;

    //Read and get the initial board

    int board_width = read_board(board);

    int *abs_to_rel_table, *rel_to_abs_table, *goals_pos;

    Node *init_node;

    int num_cells = precompute_board(
                        board_width, board, abs_to_rel_table,
                        rel_to_abs_table, goals_pos,
                        init_node);

    int board_height = board.size();

    //Precompute the neighbors matrix
    
    int (*neighbors)[4], *num_neighbors;
    
    precompute_neighbors(
                        board_height, board_width, num_cells,
                        abs_to_rel_table, rel_to_abs_table,
                        neighbors, num_neighbors
                    );

    //Make the auxiliary stack
    int *stack_arr = new int[num_cells];

    compute_area(   num_cells, neighbors,
                    num_neighbors, stack_arr, init_node );

    return 0;
}

int read_board( vector< string > &board ){
    string aux;
    board.clear();
    int n = 0;

    while( getline(cin, aux) ){
        board.push_back(aux);
        n = max( n, (int)aux.length() );
    }

    return n;
}

int precompute_board(
            int board_width, vector< string > &board,
            int *abs_to_rel_table, int *rel_to_abs_table,
            int *goals_pos, Node *init_node ) {

    return 0;
}

//...........KEVIN FUNCTIONs............



//...........END KEVIN FUNCTIONs............


//...........CARLOS FUNCTIONs............

void precompute_neighbors(
                    int board_height, int board_width, int num_cells,
                    int *abs_to_rel_table, int *rel_to_abs_table,
                    int (*neighbors)[4], int *num_neighbors
                ){

    //Create the arrays
    neighbors = new int[num_cells][4];

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
                )
                neighbors[i][ num_neighbors[i]++ ] =
                    abs_to_rel_table[ny*board_width + nx];
        }
    }
}

void compute_area(  int num_cells, 
                    int (*neighbors)[4], int *num_neighbors,
                    int *stack_arr, Node *node ){

    int arr_size = num_cells/32;
    if( num_cells%32 !=0 ) arr_size++;

    //find a valid position in the node to start with
    int p1 = 0;
    int p2 = 0;
    int mask = 1;
    while(true){
        while( p2<32 && !( node->area[p1] & mask ) ){
            p2++;
            mask<<=1;
        }
        if(p2<32) break;
        p1++;
    }

    //code for a DFS using a stack...

    int st_sz = 0; //The stack's size

    //push the first element
    stack_arr[st_sz++]= p1*32 + p2;

    while( st_sz != 0 ){

        //Take the top element and pop
        int current_cell = stack_arr[ --st_sz ];

        for(int i=0; i<num_neighbors[current_cell]; i++){

            int neig_cell = neighbors[current_cell][i];
            p1 = neig_cell/32;
            p2 = neig_cell%32;
            mask = 1<<p2;

            if( !( node->area[p1]&mask ) ){
                node->area[p1] |= mask;
                //push the element
                stack_arr[st_sz++] = neig_cell;
            }
        }
    }
}

//...........END CARLOS FUNCTIONs............


