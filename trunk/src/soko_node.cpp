#include"soko_node.h"

/**
 * This global variable is defined in "sokoban-lappis-solver.cpp"
 * and holds the number of bits an integer has in the machine
 * where this code is compiled
 */
extern int int_bits;

//Definition of static variables
int *soko_node::stack_arr;
int soko_node::num_cells;
int soko_node::arr_size;

soko_node :: soko_node(int last_pos, soko_node *father){
    this->last_pos = last_pos;
    this->father = father;
}

void soko_node :: print( int board_height, int board_width,
                    int *abs_to_rel_table ){

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


void soko_node :: compute_area( int (*neighbors)[4], int *num_neighbors){
    //Create an array for calculating the area
    this->area = new int[soko_node::arr_size];
    memset( this->area, 0, sizeof(int)*soko_node::arr_size );

    //This integer will keep the stack's size
    int st_sz = 0;

    //Push the first position: it will always be the cell
    //that was occupied by the box that the father of this node
    //pusshed to arrive to this configuration
    soko_node::stack_arr[st_sz++] = this->last_pos;

    //Mark this position on the area

    int p1 = this->last_pos/int_bits;
    int p2 = this->last_pos%int_bits;
    int mask = 1<<p2;
    this->area[p1] |= mask;

    //Start the loop
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

vector< soko_node* > *soko_node::get_sons(
            int board_width, int* abs_to_rel_table,
            int* rel_to_abs_table,
            int (*neighbors)[4], int *num_neighbors){
    
    vector< soko_node* > *my_sons = new vector< soko_node* >();

    //Check which are the reachable boxes 
    for(int p1 = 0; p1 < soko_node::arr_size; p1++){
        int mask = 1;
        for(int p2 = 0; p2 < int_bits; ++p2 ){

            if(!( this->box_pos[p1] & mask )){
                mask <<= 1;
                continue;
            }
        
            //box found! try to move it
            //check for neighbors in your area
            int box_cell = p1*int_bits + p2;

            for(int i=0; i<num_neighbors[box_cell]; i++){

                int neigh_cell = neighbors[box_cell][i];
                int n1 = neigh_cell/int_bits;
                int n2 = neigh_cell%int_bits;

                if(!( this->area[n1] & (1<<n2) ))
                    continue;
                //Check that the neighbor is not a box
                if( this->box_pos[n1] & (1<<n2) )
                    continue;

                //Found a neighbor in the area!
                //now check if oposite cell is free!

                int op_cell =
                    soko_node::find_oposite(board_width,
                                            abs_to_rel_table,
                                            rel_to_abs_table,
                                            box_cell, neigh_cell);
                //Check that the cell is reachable
                if(op_cell==-1)
                    continue;

                //check that there's no box
                int o1 = op_cell/int_bits;
                int o2 = op_cell%int_bits;
                if( this->box_pos[o1] & (1<<o2) )
                    continue;

                //Make your new son!
                soko_node *son = new soko_node(box_cell,this);                     
               

                //Recalculate the positions of the boxes
                son->box_pos = new int[soko_node::arr_size];
                memcpy(son->box_pos, this->box_pos,
                        sizeof(int)*soko_node::arr_size);

                //take the old
                son->box_pos[p1] ^= 1<<p2;
                //put the new
                son->box_pos[o1] |= 1<<o2;

                //Flood and add
                son->compute_area(neighbors, num_neighbors);
                my_sons->push_back(son);
            }

            mask <<= 1;
        }
    }
    return my_sons;
}

inline int soko_node::find_oposite(int board_width, int* abs_to_rel_table,
                            int* rel_to_abs_table, int box, int neigh){

    int abs_box = rel_to_abs_table[box]; 
    int abs_neigh = rel_to_abs_table[neigh];

    int b_y = abs_box/board_width;
    int b_x = abs_box%board_width;
    int n_y = abs_neigh/board_width;
    int n_x = abs_neigh%board_width;

    //build the vector V = Box - Neigh
    //and translate it to the origin Box
    //(Op = V + Box)
    int o_y = (b_y - n_y) + b_y;
    int o_x = (b_x - n_x) + b_x;

    //check if the oposite cell exists
    if( 0<=o_y && 0<=o_x)
        return abs_to_rel_table[o_y*board_width + o_x];

    return -1;
}

bool soko_node::is_solution(int *goals_pos){
    for(int p1 = 0; p1 < soko_node::arr_size; p1++){
        if( this->box_pos[p1] != ( this->box_pos[p1] & goals_pos[p1] ) )
            return false;
        }
    return true;
}
