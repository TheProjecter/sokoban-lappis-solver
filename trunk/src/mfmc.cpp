#include"mfmc.h"

/**
 *   ///////////////////////
 *   // MIN COST MAX FLOW //
 *   ///////////////////////
 *
 *   Authors: Frank Chu, Igor Naverniouk
 **/

/*********************
 * Min cost max flow * (Edmonds-Karp relabelling + Dijkstra)
 *********************
 * Takes a directed graph where each edge has a capacity ('cap') and a 
 * cost per unit of flow ('cost') and returns a maximum flow network
 * of minimal cost ('fcost') from s to t. USE THIS CODE FOR (MODERATELY)
 * DENSE GRAPHS; FOR VERY SPARSE GRAPHS, USE mcmf4.cpp.
 *
 * PARAMETERS:
 *      - cap (global): adjacency matrix where cap[u][v] is the capacity
 *          of the edge u->v. cap[u][v] is 0 for non-existent edges.
 *      - cost (global): a matrix where cost[u][v] is the cost per unit
 *          of flow along the edge u->v. If cap[u][v] == 0, cost[u][v] is
 *          ignored. ALL COSTS MUST BE NON-NEGATIVE!
 *      - n: the number of vertices ([0, n-1] are considered as vertices).
 *      - s: source vertex.
 *      - t: sink.
 * RETURNS:
 *      - the flow
 *      - the total cost through 'fcost'
 *      - fnet contains the flow network. Careful: both fnet[u][v] and
 *          fnet[v][u] could be positive. Take the difference.
 * COMPLEXITY:
 *      - Worst case: O(n^2*flow  <?  n^3*fcost)
 * FIELD TESTING:
 *      - Valladolid 10594: Data Flow
 * REFERENCE:
 *      Edmonds, J., Karp, R.  "Theoretical Improvements in Algorithmic
 *          Efficieincy for Network Flow Problems".
 *      This is a slight improvement of Frank Chu's implementation.
 **/
 
mfmc_solver :: mfmc_solver(int num_boxes, int num_goals){
    //Fill the capacities... where:
    //-Vertex 0 will be the source
    //-Vertex 1 to num_boxes, will be the boxes
    //-Vertex num_boxes+1 to num_boxes+num_goals, will be the goals
    //-Vertex num_boxes+num_goals+1 will be the sink

    this->num_boxes = num_boxes;
    this->num_goals = num_goals;

    this->n = num_boxes + num_goals + 2;

    this->s = 0;
    this->t = n-1;

    //Set them all to 0
    CLR( cap, 0 );

    //From the source to every box there's a capacity of 1
    for( int i=1; i <= num_boxes; i++ ){
        cap[0][i] = 1;
        //From each box to each goal
        for(int j = num_boxes+1; j<=num_boxes+num_goals;j++)
            cap[i][j] = 1;
    }

    //From each goal to the sink
    for(int j = num_boxes+1; j<=num_boxes+num_goals;j++)
        cap[j][ num_boxes+num_goals+1 ] = 1;

    //Set all costs to 0
    CLR( cost, 0 );
}

int mfmc_solver :: calc_mc( int *box_goal_distance){
    //Actuallize the cost matrix
    for(int i=0; i<num_boxes; i++)
    for(int j=0; j<num_goals; j++)
        cost[i+1][j+num_boxes+1] = box_goal_distance[i*num_goals + j];


    //for(int i=0; i<n; i++){
    //    for(int j=0; j<n; j++){
    //        cout << cap[i][j] << " ";
    //    }
    //    cout << endl;
    //}
    //cout << "................" << endl;
    //for(int i=0; i<n; i++){
    //    for(int j=0; j<n; j++){
    //        cout << cost[i][j] << " ";
    //    }
    //    cout << endl;
    //}
    //Calculate the minimum cost
    return mcmf();
}


bool mfmc_solver::dijkstra( )
{
    CLR( d, 0x3F );
    CLR( par, -1 );
    CLR( inq, -1 );
    //for( int i = 0; i < n; i++ ) d[i] = Inf, par[i] = -1;
    d[s] = qs = 0;
    inq[q[qs++] = s] = 0;
    par[s] = n;

    while( qs ) 
    {
        // get the minimum from q and bubble down
        int u = q[0]; inq[u] = -1;
        q[0] = q[--qs];
        if( qs ) inq[q[0]] = 0;
        for( int i = 0, j = 2*i + 1, t; j < qs; i = j, j = 2*i + 1 )
        {
            if( j + 1 < qs && d[q[j + 1]] < d[q[j]] ) j++;
            if( d[q[j]] >= d[q[i]] ) break;
            BUBL;
        }
        
        // relax edge (u,i) or (i,u) for all i;
        for( int k = 0, v = adj[u][k]; k < deg[u]; v = adj[u][++k] )
        {
            // try undoing edge v->u      
            if( fnet[v][u] && d[v] > Pot(u,v) - cost[v][u] ) 
                d[v] = Pot(u,v) - cost[v][par[v] = u];
        
            // try using edge u->v
            if( fnet[u][v] < cap[u][v] && d[v] > Pot(u,v) + cost[u][v] ) 
                d[v] = Pot(u,v) + cost[par[v] = u][v];
                
            if( par[v] == u )
            {
                // bubble up or decrease key
                if( inq[v] < 0 ) { inq[q[qs] = v] = qs; qs++; }
                for( int i = inq[v], j = ( i - 1 )/2, t;
                     d[q[i]] < d[q[j]]; i = j, j = ( i - 1 )/2 )
                     BUBL;
            }
        }
    }
  
    for( int i = 0; i < n; i++ ) if( pi[i] < Inf ) pi[i] += d[i];
  
    return par[t] >= 0;
}

int mfmc_solver::mcmf( )
{
    // build the adjacency list
    CLR( deg, 0 );
    for( int i = 0; i < n; i++ )
    for( int j = 0; j < n; j++ ) 
        if( cap[i][j] || cap[j][i] ) adj[i][deg[i]++] = j;
        
    CLR( fnet, 0 );
    CLR( pi, 0 );
    int flow, fcost;
    flow = fcost = 0;
    
    // repeatedly, find a cheapest path from s to t
    while( dijkstra( ) ) 
    {
        // get the bottleneck capacity
        int bot = INT_MAX;
        for( int v = t, u = par[v]; v != s; u = par[v = u] )
            bot = min(bot,fnet[v][u] ? fnet[v][u] : ( cap[u][v] - fnet[u][v] ));

        // update the flow network
        for( int v = t, u = par[v]; v != s; u = par[v = u] )
            if( fnet[v][u] ) { fnet[v][u] -= bot; fcost -= bot * cost[v][u]; }
            else { fnet[u][v] += bot; fcost += bot * cost[u][v]; }
    
        flow += bot;
    }
  
    return fcost;
}


