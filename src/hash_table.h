/**
 * KTH - Royal Institute of Technology
 * DD2380 - Artificial Intelligence
 * Autumn 2010
 *
 * Hash table
 *
 * @author  Kevin Anceau <anceau@kth.se>
 * @author  Andrea Baisero <baisero@kth.se>
 * @author  Carlos Alberto Colmenares <cacol@kth.se>
 * @author  Manuel Parras Ruiz De Azúa <mprda@kth.se>
 *
 */

#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <string>
#include <iostream>
#include <stdlib.h>
#include "soko_node.h"

using namespace std;

// for other primes around 400000 go to http://www.bigprimes.net/archive/prime/339/
#define HASH_TABLE_DIM 400009

struct node{
    soko_node *info;
    node *next;
};

class hash_table{
    public:

    node **HashTable;
    int num_nodes;
    int used_cells;
    int num_unary_cells;

    hash_table();

    void insertHash(soko_node *s_node);

    bool searchNode(soko_node *s_node);
    
    int hash(soko_node *s_node);
    
    void statistics();
};

#endif
