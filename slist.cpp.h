#include <stdlib.h>

template<class Object>
SList<Object>::SList() {
	init();
}

template<class Object>
SList<Object>::SList(const SList &rhs) {
	init();
	*this = rhs;                                   // then assign rhs to this.
}

template<class Object>
void SList<Object>::init() {
	for (int i = 0; i < LEVEL; i++) {  // for each level
									   // create the left most dummy nodes;
		header[i] = new SListNode<Object>;
		header[i]->prev = NULL;
		header[i]->down = (i > 0) ? header[i - 1] : NULL;
		header[i]->up = NULL;
		if (i > 0) header[i - 1]->up = header[i];

		// create the right most dummy nodes;
		header[i]->next = new SListNode<Object>;
		header[i]->next->next = NULL;
		header[i]->next->prev = header[i];
		header[i]->next->down = (i > 0) ? header[i - 1]->next : NULL;
		header[i]->next->up = NULL;
		if (i > 0) header[i - 1]->next->up = header[i]->next;
	}

	// reset cost.
	cost = 0;
}

template<class Object>
SList<Object>::~SList() {
	clear();                                      // delete items starting 1st
	for (int i = 0; i < LEVEL; i++) {
		delete header[i]->next;                      // delete the right most dummy
		delete header[i];                            // delete the left most dummy
	}
}

template<class Object>
bool SList<Object>::isEmpty() const {
	return (header[0]->next->next == NULL);
}

template<class Object>
int SList<Object>::size() const {
	SListNode<Object> *p = header[0]->next; // at least the right most dummy
	int size = 0;

	for (; p->next != NULL; p = p->next, ++size);
	return size;
}

template<class Object>
void SList<Object>::clear() {
	for (int i = 0; i < LEVEL; i++) {        // for each level
		SListNode<Object> *p = header[i]->next;  // get the 1st item 
		while (p->next != NULL) {              // if this is not the left most
			SListNode<Object> *del = p;
			p = p->next;                           // get the next item
			delete del;                            // delete the current item
		}

		header[i]->next = p;                     // p now points to the left most 
	}                                          // let the right most point to it
}

/*
@pre pass in a valid obj
@post insert obj in correct spot and up for a random amount of levels
*/
template<class Object>
void SList<Object>::insert(const Object &obj) {
	// right points to the level-0 item before which a new object is inserted.
	SListNode<Object> *right = searchPointer(obj);
	SListNode<Object> *up = NULL;

	if (right->next != NULL && right->item == obj)
		// there is an identical object
		return;

	//we have a pointer to the right node
	//create a node to insert
	//create a pointer to left node
	SListNode<Object> *previous = right->prev;
	SListNode<Object> *p = new SListNode<Object>();

	p->item = obj;
	//set the new nodes previous
	p->prev = previous;
	//point previous to the new node
	previous->next = p;
	//set new nodes next
	p->next = right;
	//have next point back to the node
	right->prev = p;

	//initialize int to represnet random 50/50 chance of going up a level
	int randNum = rand() % 2;
	int lvl = 0;
	while (randNum == 1 && lvl < LEVEL-1) {
		

		//make a pointer to the current object
		SListNode<Object> *pcurrentlvl = p;

		//go left until we have a link up
		while (p->up == NULL) {
			p = p->prev;
		}
		//move up
		p = p->up;
		//go right until we find the correct location
		while (p->item < p->next->item) {
			p = p->next;
		}
		//make node to insert
		SListNode<Object> *toInsert = new SListNode<Object>();
		//make pointer to next
		SListNode<Object> *next = p->next;
		toInsert->item = obj;
		//make 2 adjacent nodes point to new node
		//and make the new node point to those
		//make previous point to new node
		p->next = toInsert;
		//make new node point to previous
		toInsert->prev = p;
		next->prev = toInsert;
		toInsert->next = next;
		toInsert->down = pcurrentlvl;
		pcurrentlvl->up = toInsert;

		//randomize the value to see if we go up a level
		randNum = rand() % 2;
		lvl++;
		p = toInsert;
	}

}

template<class Object>
bool SList<Object>::find(const Object &obj) {
	// points to the level-0 item close to a given object
	SListNode<Object> *p = searchPointer(obj);

	return (p->next != NULL && p->item == obj);     // true if obj was found
}

template<class Object>
SListNode<Object> *SList<Object>::searchPointer(const Object &obj) {
	SListNode<Object> *p = header[LEVEL - 1];     // start from the top left
	while (p->down != NULL) {                   // toward level 0
		p = p->down;                                // shift down once
		cost++;

		if (p->prev == NULL) {                    // at the left most item
			if (p->next->next == NULL)              // no intermediate items
				continue;
			else {                                    // some intermadiate items
				if (p->next->item <= obj)             // if 1st item <= obj
					p = p->next;                          // shift right to item 1
				cost++;
			}
		}

		while (p->next->next != NULL && p->next->item <= obj) {
			// shift right through intermediate items as far as the item value <= obj
			p = p->next;
			cost++;
		}
	}

	// now reached the bottom. shift right once if the current item < obj
	if (p->prev == NULL || p->item < obj) {
		p = p->next;
		cost++;
	}
	return p; // return the pointer to an item >= a given object.
}

/*
@pre pass in valid object
@post remove all instances of the object from list at all levels
*/
template<class Object>
void SList<Object>::remove(const Object &obj) {
	// p points to the level-0 item to delete
	SListNode<Object> *p = searchPointer(obj);

	// validate if p is not the left most or right most and exactly contains the
	// item to delete
	if (p->prev == NULL || p->next == NULL || p->item != obj)
		return;
	//not the cleanest way to remove as this lacks garbage management
	//however very simple to implement
	//simply grab two adjacent nodes and make them point to each other
	//losing the current node
	//for larger cases should manually delete nodes, but deconstructor not present
	SListNode<Object> *left = p->prev;
	SListNode<Object> *right = p->next;
	left->next = right;
	right->prev = left;
	//while we can keep moving up, repeat
	while (p->up != NULL) {
		p = p->up;
		SListNode<Object> *l = p->prev;
		SListNode<Object> *r = p->next;
		l->next = r;
		r->prev = l;
	}
	
}

template<class Object>
const SList<Object> &SList<Object>::operator=(const SList &rhs) {
	if (this != &rhs) { // avoid self-assignment
		clear();           // deallocate old items

		int index;
		SListNode<Object> *rnode;
		for (index = 0, rnode = rhs.header[0]->next; rnode->next != NULL;
			rnode = rnode->next, ++index)
			insert(rnode->item);

		cost = rhs.cost;
	}
	return *this;
}

template<class Object>
int SList<Object>::getCost() const {
	return cost;
}

template<class Object>
void SList<Object>::show() const {
	cout << "contents:" << endl;
	for (SListNode<Object> *col = header[0]; col != NULL; col = col->next) {
		SListNode<Object> *row = col;
		for (int level = 0; row != NULL && level < LEVEL; level++) {
			if (row->prev == NULL)
				cout << "-inf\t";
			else if (row->next == NULL)
				cout << "+inf\t";
			else
				cout << row->item << "\t";
			row = row->up;
		}
		cout << endl;
	}
}

#pragma once
