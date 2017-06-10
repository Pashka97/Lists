// --------------------------transposelist.cpp.h------------------------------
// Pavel Samsonov CSS 342
// 05 May 2017
// ---------------------------------------------------------------------------

#include <iostream>
#include "dlist.h"

using namespace std;


/*
Precondition: valid parameter passed in
Postcondition: If found, swaps this object with the one in front of it,
else returns -1
*/
template<class Object>
int TransposeList<Object>::find(const Object &obj) {
	DListNode<Object> *found = DList<Object>::header->next;

	int i = 0;
	for (; found != NULL && found->item != obj; found = found->next, ++i)
		++DList<Object>::cost;

	if (found == NULL)
		return -1; // not found

	if (found == DList<Object>::header->next)
		return 0;  // no need to swap

	this->remove(found->item);		// remove found from the current position
	this->insert(obj, i - 1);			// insert found before previous

	return i;
}

