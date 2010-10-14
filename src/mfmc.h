#ifndef _SOKO_MFMC_H
#define _SOKO_MFMC_H

#include<iostream>
#include <string.h>
#include <limits.h>

// the maximum number of vertices + 1
#define NN 1024

#define CLR(a, x) memset( a, x, sizeof( a ) )
#define Inf (INT_MAX/2)
#define BUBL { \
    t = q[i]; q[i] = q[j]; q[j] = t; \
    t = inq[q[i]]; inq[q[i]] = inq[q[j]]; inq[q[j]] = t; }

// Dijkstra's using non-negative edge weights (cost + potential)
#define Pot(u,v) (d[u] + pi[u] - pi[v])

using namespace std;


class mfmc_solver{
  private:
    
    // adjacency matrix 
    int cap[NN][NN];

    // cost per unit of flow matrix 
    int cost[NN][NN];

    // flow network and adjacency list
    int fnet[NN][NN], adj[NN][NN], deg[NN];

    // Dijkstra's predecessor, depth and priority queue
    int par[NN], d[NN], q[NN], inq[NN], qs;

    // Labelling function
    int pi[NN];

    // The number of vertex   
    int n;

    // The source and sink
    int s, t;

    //Number of boxes and goals
    int num_boxes, num_goals;

    bool dijkstra( );

    int mcmf( );

  public:
    
    mfmc_solver(int num_boxes, int num_goals);

    int calc_mc( int *box_goal_distance);
};



#endif
