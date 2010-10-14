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
#include "deadlock_check.h"

extern int int_bits;

void init_deadlock_list(int *rel_to_abs_table, int *abs_to_rel_table,
                        int *deadlock_list, int (*neighbors)[4], 
                        int *&num_neighbors, int *goals,
                        int num_cell, int board_width){
    
    int direction[2][2] = {{0,0},{0,0}};
    
    //test every cells to decide whether it's a deadlock or not
    //this loop only determine edges deadlock
    for(int i=0; i<num_cell;i++){
        //if it is a goal, it's not a deadlock
        int p1 = i / int_bits;
        int p2 = i % int_bits;
        if (goals[p1] & (1<<p2)) continue;
        
        //if 1 neighbors, it's a deadlock
        if (num_neighbors[i] == 1) add_to_list(deadlock_list, i);
        
        //if 2 neigbhors, we have to know more to decide
        if (num_neighbors[i] == 2){
            //calculate absolute position of each cells
            int abs_pos_0 = rel_to_abs_table[neighbors[i][0]];
            int abs_pos_1 = rel_to_abs_table[neighbors[i][1]];
            int cy0 = abs_pos_0 / board_width;
            int cx0 = abs_pos_0 % board_width;
            int cy1 = abs_pos_1 / board_width;
            int cx1 = abs_pos_1 % board_width;
            
            if ((cx0 != cx1) && (cy0 != cy1)){
                //add this cell to the deadlock list
                add_to_list(deadlock_list, i);
            }
        }
    }
    
    //test again every cells to determine
    //dead ends beside walls
    for(int i=0; i<num_cell;i++){
        //if 2 neigbhors, we have to know more to decide
        if (num_neighbors[i] == 2){
            //calculate absolute position of each cells
            int abs_pos_0 = rel_to_abs_table[neighbors[i][0]];
            int abs_pos_1 = rel_to_abs_table[neighbors[i][1]];
            int cy0 = abs_pos_0 / board_width;
            int cx0 = abs_pos_0 % board_width;
            int cy1 = abs_pos_1 / board_width;
            int cx1 = abs_pos_1 % board_width;
            
            if ((cx0 != cx1) && (cy0 != cy1)){
                int abs_pos = rel_to_abs_table[i];
                int cy = abs_pos / board_width;
                int cx = abs_pos % board_width;
                
                //if it is a goal, no need to test walls from here
                if (goals[i / int_bits] & (1<<(i % int_bits))) continue;
                
                //check for the whole line beside the wall
                direction[0][0] = cx0 - cx;
                direction[0][1] = cy0 - cy;
                direction[1][0] = cx1 - cx;
                direction[1][1] = cy1 - cy;
                
                //test direction 0
                init_deadlock_beside_wall(rel_to_abs_table,
                                          abs_to_rel_table,
                                          deadlock_list, goals,
                                          direction, 0, i ,board_width);
                
                
                //test direction 1
                init_deadlock_beside_wall(rel_to_abs_table,
                                          abs_to_rel_table,
                                          deadlock_list, goals,
                                          direction, 1, i ,board_width);
            }
        }
    }
}

bool init_deadlock_beside_wall(int *rel_to_abs_table,
                               int *abs_to_rel_table,
                               int *deadlock_list, 
                               int *goals,
                               int direction[2][2], int dir,
                               int from_cell, int board_width){
    
    
    //calculate absolute position of cells
    //previous cell
    int abs_pos = rel_to_abs_table[from_cell];
    int cy = abs_pos / board_width;
    int cx = abs_pos % board_width;
    
    //new tested cell
    int nx = cx + direction[dir][0];
    int ny = cy + direction[dir][1];
    int nc = abs_to_rel_table[ny*board_width + nx];
    
    //is nc already a known deadlock ?
    int p1 = nc / int_bits;
    int p2 = nc % int_bits;
    if (deadlock_list[p1] & (1<<p2)) return true;
    
    //is nc a goal ?
    if (goals[p1] & (1<<p2)) return false;
    
    //is the nc still beside a wall ?
    int wall_dir = (dir == 0 ? 1 : 0);
    int wallx = nx - direction[wall_dir][0];
    int wally = ny - direction[wall_dir][1];
    
    if (abs_to_rel_table[wally*board_width + wallx] != -1) return false;
    
    //continue the tests :
    //if the next cell in that direction is a deadlock,
    //then nc is also a deadlock. Then, we add nc return true
    if (init_deadlock_beside_wall(rel_to_abs_table,
                                  abs_to_rel_table,
                                  deadlock_list, goals,
                                  direction, dir, nc, board_width)){
        add_to_list(deadlock_list, nc);
        return true;
    }else{
        return false;
    }
}

bool is_deadlock(int *rel_to_abs_table, int *abs_to_rel_table,
                 int *deadlock_list, int (*neighbors)[4], 
                 int *&num_neighbors, soko_node *node,
                 int num_cell, int board_height, int board_width,
                int* goals_pos){    
    
/*    node->print(board_height,board_width,
                    abs_to_rel_table);
    cout << endl;*/
    //intersection between deadlock list and box position
    //if a box is on a deadlock cell, return true
    int lists_size = num_cell/int_bits;
    if (num_cell%int_bits != 0) lists_size++;
    
    for(int i=0; i<lists_size; i++)
        if (deadlock_list[i] & node->box_pos[i])
            return true;
    
//    cout << "WAS NOT A STATIC DEADLOCK" << endl;

    if(is_freeze_deadlock(node,num_neighbors,
                        rel_to_abs_table,abs_to_rel_table,
                        num_cell,board_width, goals_pos))
        return true;
    
//    cout << "WAS NOT A FREEZE DEADLOCK" << endl << endl;
    return false;
}

bool is_freeze_deadlock(soko_node *node, int *num_neighbors,
                        int *rel_to_abs_table, int *abs_to_rel_table,
                        int num_cell, int board_width, int* goals_pos) {
    int lists_size = num_cell/int_bits;
    if (num_cell%int_bits != 0) lists_size++;

    int box_abs=rel_to_abs_table[node->last_pos];
    int box_x=box_abs%board_width;
    int box_y=box_abs/board_width;
    
    bool swap=false;
    int inv_i=1;
    int inv_j=1;
    switch(node->push_dir) {
        case 'U':
            box_y--;  
            break;
        case 'D':
            box_y++;
            inv_i=-1;
            inv_j=-1;
            break;
        case 'L':
            box_x--;
            swap=true;
            inv_i=-1;
            break;
        case 'R':
            box_x++;
            swap=true;
            inv_j=-1;
            break;
    }
    int box_rel=abs_to_rel_table[box_y*board_width+box_x];

    bool box[3][3], wall[3][3], goal[3][3];

    for(int i=-1;i<2;i++) {
        for(int j=-1;j<2;j++) {
            int y=box_y+inv_i*(swap?j:i);
            int x=box_x+inv_j*(swap?i:j);
            int xy_rel=abs_to_rel_table[y*board_width+x];
            box[i+1][j+1]=(xy_rel!=-1
                            && (node->box_pos[xy_rel/int_bits]
                                & (1<<(xy_rel%int_bits))));
            wall[i+1][j+1]=(xy_rel==-1);
            goal[i+1][j+1]=(xy_rel!=-1
                            && (goals_pos[xy_rel/int_bits]
                                & (1<<(xy_rel%int_bits))));
        }
    }
    
    if(!box[0][1] && !wall[0][1])
        return false;

    if((box[0][0] || wall[0][0])
        && (box[1][0] || wall[1][0])
        && (box[0][1] || wall[0][1])
        && (!goal[1][1]
            || !wall[0][0] && !goal[0][0]
            || !wall[1][0] && !goal[1][0]
            || !wall[0][1] && !goal[0][1]))
            return true;

    if((box[0][1] || wall[0][1])
        && (box[0][2] || wall[0][2])
        && (box[1][2] || wall[1][2])
        && (!goal[1][1]
            || !wall[0][1] && !goal[0][1]
            || !wall[0][2] && !goal[0][2]
            || !wall[1][2] && !goal[1][2]))
            return true;

    if(box[0][1] && ((wall[0][2] && wall[1][0]) || (wall[0][0] && wall[1][2]))
        && (!goal[1][1] || !goal[0][1]))
        return true;

    if(wall[0][1] && ((box[1][0] && (!goal[1][1]||!goal[1][0]) && wall[2][0])
                    ||(box[1][2] && (!goal[1][1]||!goal[1][2]) && wall[2][2])))
        return true;

    return false;
}
