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

using namespace std;

hash_table :: hash_table(int size){
	this->HashTable = new node[size];
}

void hash_table :: insertHash(int pos, soko_node info){
	// This position of the Hash table has already node(s)
	if (this->HashTable[pos].next != NULL) {
		cout << "insertHash: Inserts in a position that was not empty" << endl;
		node* aux = this->HashTable[pos].next;
		node* node_aux = (node*)malloc(sizeof(node));
		node_aux->info = info;
		node_aux->next = aux;
		this->HashTable[pos].next = node_aux;
	}
	// This position of the Hash table is empty
	else {
		cout << "insertHash: Inserts in a empty position" << endl;
		node* node_aux = (node*)malloc(sizeof(node));
		node_aux->info = info;
		node_aux->next = NULL;
		this->HashTable[pos].next = node_aux;
	}

}

bool hash_table :: equalSokoNode (soko_node *soko1, soko_node *soko2){
	if ((soko1->area == soko2->area) && (soko1->box_pos == soko2->box_pos) && (soko1->last_pos == soko2->last_pos)) {
		//cout << "equalSokoNode: Both soko_nodes are equal" << endl;
		return true;
	}
	else {
		//cout << "equalSokoNode: The soko_nodes are not equal" << endl;
		return false;
	}

}

// This function returns 'false' if the information is not in the table, and 'true' if it is in it
bool hash_table :: searchNode(int pos, soko_node* info){
	bool flag = false;
	// This position of the Hash table has already node(s)
	if (this->HashTable[pos].next != NULL) {
		node* aux = this->HashTable[pos].next;
		node* aux2;
		do {
			aux2 = aux;
			if (equalSokoNode(&aux->info, info)) {
				cout << "searchNode: The element exists in the table" << endl;
				//flag = true;
				return true;
			}
			if (aux->next != NULL) {
				//cout << "searchNode: Advances to the next element in the position" << endl;
				aux = aux->next;
			}
		} while (aux2->next != NULL);
		return flag;
	}
	//This position of the Hash table is empty, so the information we are looking for is not there
	else {
		cout << "searchNode: The element does not exist in the table" << endl;
		return false;
	}
}

int hash_table :: hash(soko_node *info){
	int area = *(info->area);
	int box_pos = *(info->box_pos);
	int x_or = area ^ box_pos;
	int aux = area * box_pos * x_or;
	aux = aux % sizeof(this->HashTable);
	return aux;
}

void hash_table :: statistics(){
	int counter_occupied = 0;
	for (int i = 0; i < sizeof(this->HashTable); i++) {
		if (this->HashTable[i].next != NULL) {
			counter_occupied++;
		}
	}
	cout << "statistics: The Hash Table has " << counter_occupied << " positions occupied of a total of " << sizeof(this->HashTable) << endl;

	int max = 0;
	int pos = 0;
	int counter = 0;
	for (int i = 0; i < sizeof(this->HashTable); i++) {
		if (this->HashTable[i].next != NULL) {
			node* aux = this->HashTable[pos].next;
			node* aux2;
			do {
				aux2 = aux;
				counter++;
				if (aux->next != NULL) {
					aux = aux->next;
				}
			} while (aux2->next != NULL);
			
			if (counter > max) {
				max = counter;
				pos = i;
			}
			counter = 0;
		}
	}
	
	cout << "statistics: The position most occupied is " << pos << " with " << max << " soko_nodes stored." << endl;
}