// --------------------------mtflist.cpp.h------------------------------------
// Pavel Samsonov CSS 342
// 05 May 2017
// ---------------------------------------------------------------------------

#include <iostream>
#include "dlist.h"

using namespace std;

/*
Precondition: valid parameter passed in
Postcondition: If found, moves object to front of list, else returns -1
*/
template<class Object>
int MtfList<Object>::find(const Object &obj) {
	DListNode<Object> *top = DList<Object>::header->next;
	DListNode<Object> *found = top;

	for (; found != NULL && found->item != obj; found = found->next)
		++DList<Object>::cost;

	if (found == NULL)
		return -1; // not found

	if (found == top)
		return 0;  // no need to move to front     

	this->remove(found->item);		// remove found from the current position
	this->insert(obj, 0);				// insert found between header and top

	return 0;

}


