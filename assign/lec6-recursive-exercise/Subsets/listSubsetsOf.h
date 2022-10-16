#ifndef LISTSUBSETSOF_H
#define LISTSUBSETSOF_H

#include "Vector.h"
#include <iostream>

void listSubsetsOfIterate(Vector<int> elems);
void listSubsetsOf(Vector<int> elems);
void listSubsetsOfHelper(Vector<int> elems, Vector<int> sofar);
void listSubsetsOfHelper(Vector<int> &elems, int start, int end, Vector<int> sofar);

#endif // LISTSUBSETSOF_H
