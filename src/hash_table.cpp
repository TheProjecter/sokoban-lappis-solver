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

hash_table :: hash_table(){
    this->HashTable = new node*[HASH_TABLE_DIM]();

    if(this->HashTable[20]!=NULL)
        cout << "ERROR! INITIALIZE THE HASH TABLE TO NULL!!!!" << endl;

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
    
    if(this->HashTable[pos]==NULL) {
        //cout << "insertHash: Inserts in a empty position" << endl;
        this->HashTable[pos]=aux;
        this->used_cells++;
        this->num_unary_cells++;
    }
    else {
        if(this->HashTable[pos]->next==NULL)
            this->num_unary_cells--;
        //cout << "insertHash: Inserts in a position that was not empty" << endl;        
        aux->next=this->HashTable[pos];
        this->HashTable[pos]=aux;
        
    }
    this->num_nodes++;
}

// This function returns 'false' if the information is not in the table, and 'true' if it is in it
bool hash_table :: searchNode(soko_node* s_node) {
    int pos=hash(s_node);
    
    for(node* pcorr=this->HashTable[pos];pcorr!=NULL;pcorr=pcorr->next) {
        if(soko_node::equal(pcorr->info,s_node)) {
            //cout << "searchNode: The element exists in the table" << endl;
            return true;            
        }
    }
    //cout << "searchNode: The element does not exist in the table" << endl;
    this->insertHash(s_node);
    return false;
}

int hash_table :: hash(soko_node *s_node){
    long int h=0;
    for(int i=0;i<soko_node::arr_size;i++)
        h=(h<<int_bits) + s_node->area[i]^s_node->box_pos[i];

    h^=(h>>4);
    h=(h^0xdeadbeef)+(h<<5);
    h^=(h>>11);

    return h % HASH_TABLE_DIM;
}

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
    cout << "% used cells:\t" << (100.0*used_cells/HASH_TABLE_DIM) << endl;
    cout << "avg num pointers in used cell:\t" << ((float)num_nodes/used_cells) << endl;
    cout << "num pointers in biggest cell:\t" << max << endl;
    cout << "% used cells with only 1 entry:\t" << (100.0*num_unary_cells/HASH_TABLE_DIM) << endl << endl;
}
