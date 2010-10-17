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

// returns true if the node is in a deadlock situation
bool is_deadlock(int *rel_to_abs_table, int *abs_to_rel_table,
                 int *deadlock_list, int (*neighbors)[4], 
                 int *&num_neighbors, soko_node *node,
                 int num_cell, int board_height, int board_width,
                int* goals_pos){
    //intersection between deadlock list and box position
    //if a box is on a deadlock cell, return true
    int lists_size = num_cell/int_bits;
    if (num_cell%int_bits != 0) lists_size++;
    
    for(int i=0; i<lists_size; i++)
        if (deadlock_list[i] & node->box_pos[i])
            return true;

    // if the node is in a freeze deadlock situation, return true
    if(is_freeze_deadlock(node,num_neighbors,
                        rel_to_abs_table,abs_to_rel_table,
                        num_cell,board_width, goals_pos))
        return true;
    return false;
}

//checks if a node is in a freeze deadlock
bool is_freeze_deadlock(soko_node *node, int *num_neighbors,
                        int *rel_to_abs_table, int *abs_to_rel_table,
                        int num_cell, int board_width, int* goals_pos) {
    int lists_size = num_cell/int_bits;
    if (num_cell%int_bits != 0) lists_size++;

    int box_abs=rel_to_abs_table[node->last_pos];
    int box_x=box_abs%board_width;
    int box_y=box_abs/board_width;
    
    // to check for freeze deadlocks i create 3x3 matrixes which represent
    // the imediate surrounding of the last pushed box this matrix is created
    // in a way to eliminate different rotations: the box is always in [1][1]
    // and the "man" should be after pushing the box is always in [2][1]

    // initialize variables that permit to eliminate all different rotations
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

    // actually create the matrixes which neutralize all rotations
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
    
    // if the cell which is in front of the box [1][1] is empty,
    // it can't be a freeze deadlock
    if(!box[0][1] && !wall[0][1])
        return false;

    // studies the situation where 4 boxes/walls are arranged
    // in four nearby cells: none of them can be moved anymore
    if((box[0][0] || wall[0][0])
        && (box[1][0] || wall[1][0])
        && (box[0][1] || wall[0][1])
        && (!goal[1][1]
            || !wall[0][0] && !goal[0][0]
            || !wall[1][0] && !goal[1][0]
            || !wall[0][1] && !goal[0][1]))
            return true;

    // other situation analogous to the last one
    if((box[0][1] || wall[0][1])
        && (box[0][2] || wall[0][2])
        && (box[1][2] || wall[1][2])
        && (!goal[1][1]
            || !wall[0][1] && !goal[0][1]
            || !wall[0][2] && !goal[0][2]
            || !wall[1][2] && !goal[1][2]))
            return true;

    // another kind
    if(box[0][1] && ((wall[0][2] && wall[1][0]) || (wall[0][0] && wall[1][2]))
        && (!goal[1][1] || !goal[0][1]))
        return true;

    // last kind
    if(wall[0][1] && ((box[1][0] && (!goal[1][1]||!goal[1][0]) && wall[2][0])
                    ||(box[1][2] && (!goal[1][1]||!goal[1][2]) && wall[2][2])))
        return true;

    // this code works and looks for one last kind of freeze deadlocks
    // it's when boxes create a "ladder" shape, in which none of them can be moved
    // this kind of deadlock is so rare that including this test gets worse
    // performances by a few seconds because there is more time wasted on making
    // this check than the time gained thanks to this.
/*
    bool tl_dl=true;
    bool tr_dl=true;
    bool bl_dl=true;
    bool br_dl=true;

    bool tl_goal=true;
    bool tr_goal=true;
    bool bl_goal=true;
    bool br_goal=true;
    
    if(!goal[1][1] || (box[0][1]&&!goal[0][1])) {
        tl_goal=false;
        tr_goal=false;
        bl_goal=false;
        br_goal=false;
    }
    
    int top_y=-1;
    int bottom_y=1;
    int right_x=1;
    int left_x=-1;

    bool curr_wall;
    bool curr_box;
    bool curr_goal;
    bool next_wall;
    bool next_box;
    bool next_goal;

    int curr_x;
    int curr_y;
    int curr_rel;
    int next_x;
    int next_y;
    int next_rel;

    if(box[0][1]) {
        // top-left
        curr_y=box_y;
        curr_x=box_x;
        do {
            curr_y+=inv_i*(swap?left_x:top_y);
            curr_x+=inv_j*(swap?top_y:left_x);

            next_y=curr_y+inv_i*(swap?0:-1);
            next_x=curr_x+inv_j*(swap?-1:0);

            curr_rel=abs_to_rel_table[curr_y*board_width+curr_x];
            next_rel=abs_to_rel_table[next_y*board_width+next_x];

            curr_wall=(curr_rel==-1);
            curr_box=(curr_rel!=-1
                        && (node->box_pos[curr_rel/int_bits]
                                & (1<<(curr_rel%int_bits))));
            curr_goal=(curr_rel!=-1
                        && (goals_pos[curr_rel/int_bits]
                                & (1<<(curr_rel%int_bits))));

            next_wall=(next_rel==-1);
            next_box=(next_rel!=-1
                        && (node->box_pos[next_rel/int_bits]
                                & (1<<(next_rel%int_bits))));
            next_goal=(next_rel!=-1
                        && (goals_pos[next_rel/int_bits]
                                & (1<<(next_rel%int_bits))));
            if(!(curr_wall || (curr_box && (next_wall || next_box))))
                tl_dl=false;
            if((curr_box && !curr_goal) || (curr_box && next_box && !next_goal))
                tl_goal=false;
        } while(curr_box && next_box);

        // top-right
        curr_y=box_y;
        curr_x=box_x;
        do {
            curr_y+=inv_i*(swap?right_x:top_y);
            curr_x+=inv_j*(swap?top_y:right_x);

            next_y=curr_y+inv_i*(swap?0:-1);
            next_x=curr_x+inv_j*(swap?-1:0);

            curr_rel=abs_to_rel_table[curr_y*board_width+curr_x];
            next_rel=abs_to_rel_table[next_y*board_width+next_x];

            curr_wall=(curr_rel==-1);
            curr_box=(curr_rel!=-1
                        && (node->box_pos[curr_rel/int_bits]
                                & (1<<(curr_rel%int_bits))));
            curr_goal=(curr_rel!=-1
                        && (goals_pos[curr_rel/int_bits]
                                & (1<<(curr_rel%int_bits))));

            next_wall=(next_rel==-1);
            next_box=(next_rel!=-1
                        && (node->box_pos[next_rel/int_bits]
                                & (1<<(next_rel%int_bits))));
            next_goal=(next_rel!=-1
                        && (goals_pos[next_rel/int_bits]
                                & (1<<(next_rel%int_bits))));
            if(!(curr_wall || (curr_box && (next_wall || next_box))))
                tr_dl=false;
            if((curr_box && !curr_goal) || (curr_box && next_box && !next_goal))
                tr_goal=false;
        } while(curr_box && next_box);
    }
    else if(!wall[0][1]) {
        tl_dl=false;
        tr_dl=false;
    }

    if(tl_dl) {
        // bottom-right
        curr_y=box_y;
        curr_x=box_x;
        do {
            curr_y+=inv_i*(swap?right_x:bottom_y);
            curr_x+=inv_j*(swap?bottom_y:right_x);

            next_y=curr_y+inv_i*(swap?0:-1);
            next_x=curr_x+inv_j*(swap?-1:0);

            curr_rel=abs_to_rel_table[curr_y*board_width+curr_x];
            next_rel=abs_to_rel_table[next_y*board_width+next_x];

            curr_wall=(curr_rel==-1);
            curr_box=(curr_rel!=-1
                        && (node->box_pos[curr_rel/int_bits]
                                & (1<<(curr_rel%int_bits))));
            curr_goal=(curr_rel!=-1
                        && (goals_pos[curr_rel/int_bits]
                                & (1<<(curr_rel%int_bits))));

            next_wall=(next_rel==-1);
            next_box=(next_rel!=-1
                        && (node->box_pos[next_rel/int_bits]
                                & (1<<(next_rel%int_bits))));
            next_goal=(next_rel!=-1
                        && (goals_pos[next_rel/int_bits]
                                & (1<<(next_rel%int_bits))));
            if(!(next_wall || (next_box && (curr_wall || curr_box))))
                br_dl=false;
            if((next_box && curr_box && !curr_goal) || (next_box && !next_goal))
                br_goal=false;
        } while(curr_box && next_box);
        if(br_dl && (!tl_goal || !br_goal))
            return true;
    }

    if(tr_dl) {
        // bottom-left
        curr_y=box_y;
        curr_x=box_x;
        do {
            curr_y+=inv_i*(swap?left_x:bottom_y);
            curr_x+=inv_j*(swap?bottom_y:left_x);

            next_y=curr_y+inv_i*(swap?0:-1);
            next_x=curr_x+inv_j*(swap?-1:0);

            curr_rel=abs_to_rel_table[curr_y*board_width+curr_x];
            next_rel=abs_to_rel_table[next_y*board_width+next_x];

            curr_wall=(curr_rel==-1);
            curr_box=(curr_rel!=-1
                        && (node->box_pos[curr_rel/int_bits]
                                & (1<<(curr_rel%int_bits))));
            curr_goal=(curr_rel!=-1
                        && (goals_pos[curr_rel/int_bits]
                                & (1<<(curr_rel%int_bits))));

            next_wall=(next_rel==-1);
            next_box=(next_rel!=-1
                        && (node->box_pos[next_rel/int_bits]
                                & (1<<(next_rel%int_bits))));
            next_goal=(next_rel!=-1
                        && (goals_pos[next_rel/int_bits]
                                & (1<<(next_rel%int_bits))));

            if(!(next_wall || (next_box && (curr_wall || curr_box))))
                bl_dl=false;
            if((next_box && curr_box && !curr_goal) || (next_box && !next_goal))
                bl_goal=false;
        } while(curr_box && next_box);
        if(bl_dl && (!tr_goal || !bl_goal))
            return true;
    }
*/
    return false;
}
