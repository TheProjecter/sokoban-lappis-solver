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

#include "hash_table.h"

extern int int_bits;

using namespace std;

// constructor of the hashtable, allocates the array
// and initializes variables used for statictics
hash_table :: hash_table(){
    this->HashTable = new node*[HASH_TABLE_DIM]();

    this->num_nodes=0;
    this->used_cells=0;
    this->num_unary_cells=0;
}

void hash_table :: insertHash(soko_node *s_node){
    // This position of the Hash table has already node(s)
    int pos=hash(s_node);
    node *aux=(node*)malloc(sizeof(node));
    aux->info=s_node;
    aux->next=NULL;
    
    // if the cell is empty, fill it for the first time
    if(this->HashTable[pos]==NULL) {
        this->HashTable[pos]=aux;
        this->used_cells++;
        this->num_unary_cells++;
    }
    else {
        // otherwise add the new node in the head
        if(this->HashTable[pos]->next==NULL)
            this->num_unary_cells--;   
        aux->next=this->HashTable[pos];
        this->HashTable[pos]=aux;
        
    }
    this->num_nodes++;
}

// This function returns 'false' if the node is not in the table
// and 'true' if it is in it
bool hash_table :: searchNode(soko_node* s_node) {
    int pos=hash(s_node);
    
    // search in the linked list of all elements with the same hash value
    for(node* pcorr=this->HashTable[pos];pcorr!=NULL;pcorr=pcorr->next)
        if(soko_node::equal(pcorr->info,s_node))
            return true;            
    // it also automatically inserts the node in the hash
    this->insertHash(s_node);
    return false;
}

// hash function for the hash table
int hash_table :: hash(soko_node *s_node){
    long int h=0;
    // hash function uses all information from the nodes
    for(int i=0;i<soko_node::arr_size;i++)
        h=(h<<int_bits/2) + s_node->area[i]^s_node->box_pos[i];

    // and scrambles it a little
    h^=(h>>4);
    h=(h^0xdeadbeef)+(h<<5);
    h^=(h>>11);

    return h % HASH_TABLE_DIM;
}

// prints various statistics about the hash table
void hash_table :: statistics(){
    int pos=-1;
    int max=0;
    for (int i=0;i<HASH_TABLE_DIM;i++) {
        int counter=0;
        for(node* pcorr=this->HashTable[i];pcorr!=NULL;pcorr=pcorr->next)
            counter++;
        if(counter>max) {
            pos=i;
            max=counter;
        }
    }
    
    cout << endl << "HashTable" << endl;
    cout << "---------" << endl;
    cout << "size:\t" << HASH_TABLE_DIM << endl;
    cout << "num nodes:\t" << num_nodes << endl;
    cout << "used cells:\t" << (100.0*used_cells/HASH_TABLE_DIM) << "%" << endl;
    cout << "avg num pointers in used cell:\t" << ((float)num_nodes/used_cells) << endl;
    cout << "num pointers in biggest cell:\t" << max << endl;
    cout << "used cells with only 1 entry:\t" << (100.0*num_unary_cells/used_cells) << "%" << endl << endl;
}
