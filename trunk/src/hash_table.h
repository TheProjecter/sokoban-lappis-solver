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

#include <string>
#include <iostream>
#include <stdlib.h>
#include "soko_node.h"

using namespace std;

struct node{
	soko_node info;
	node* next;
};

class hash_table{
	public:

	node *HashTable;

	void insertHash(int pos, soko_node info);

	bool searchNode(int pos, soko_node* info);
	
	bool equalSokoNode (soko_node *soko1, soko_node *soko2);
	
	int hash(soko_node *info);
	
	void statistics();

	hash_table(int size);
};