#include"soko_node.h"

/**
 * This global variable is defined in "sokoban-lappis-solver.cpp"
 * and holds the number of bits an integer has in the machine
 * where this code is compiled
 */
extern int int_bits;


soko_node :: soko_node(soko_node *father){
    this->father = father;
}

void soko_node :: print( int board_height, int board_width,
                    int num_cells, int *abs_to_rel_table ){

    //Iterate through the abs_to_rel_table
    for(int i=0; i<board_height ; i++){
        for(int j=0; j<board_width ; j++){

            int rel_value = abs_to_rel_table[i*board_width + j];
            
            if( rel_value == -1 ){
                cout << "#";
            }
            else {
                int p1 = rel_value/int_bits;
                int p2 = rel_value%int_bits;

                bool box = this->box_pos[p1] & (1<<p2);
                bool guy = this->area[p1] & (1<<p2);

                if(box && guy)  cout << "x";
                else if(guy)    cout << ".";
                else if(box)    cout << "$";
                else            cout << " ";
            }
        }
        cout << endl;
    }
}


void soko_node :: compute_area(  int num_cells, 
                    int (*neighbors)[4], int *num_neighbors){

    //find a valid position in the node to start with
    int p1 = 0;
    int p2 = 0;
    int mask = 1;
    while(true){
        while( p2<int_bits && !( this->area[p1] & mask ) ){
            p2++;
            mask<<=1;
        }
        if(p2<int_bits) break;
        p1++;
    }

    //code for a DFS using a stack...

    int st_sz = 0; //The stack's size

    //push the first element
    soko_node::stack_arr[st_sz++]= p1*int_bits + p2;

    while( st_sz != 0 ){

        //Take the top element and pop
        int current_cell = soko_node::stack_arr[ --st_sz ];

        //Check neighbors
        for(int i=0; i<num_neighbors[current_cell]; i++){

            int neig_cell = neighbors[current_cell][i];
            p1 = neig_cell/int_bits;
            p2 = neig_cell%int_bits;
            mask = 1<<p2;

            if( !( this->area[p1]&mask ) ){
                this->area[p1] |= mask;

                //push the element if there's not a box
                //on it
                if( !( this->box_pos[p1]&mask ) )
                    soko_node::stack_arr[st_sz++] = neig_cell;
            }
        }
    }
}

vector< soko_node > soko_node::get_sons( int (*neighbors)[4],
                                         int *num_neighbors){
    
    //Check which are the reacheble boxes 
    int p1 = 0;
    int p2 = 0;
    int mask = 1;

    while( p1   ){
        if( p2<int_bits && !( this->box_pos[p1] & mask ) ){
            p2++;
            mask<<=1;
        }
        if(p2<int_bits) break;
        p1++;
    }

}
