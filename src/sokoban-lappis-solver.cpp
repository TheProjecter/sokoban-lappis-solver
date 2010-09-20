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



//...........END CARLOS FUNCTIONs............


