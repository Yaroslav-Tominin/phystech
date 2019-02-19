#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __LIST__
#define __LIST__
typedef struct ListElement
{
	void* element;
	struct ListElement* prev;
	struct ListElement* next;
}ListElement,*pListElement;
typedef struct List
{
	pListElement start;
	pListElement end;
}List,*pList;
typedef struct ListIter
{
	pListElement value;
	pListElement next;
} ListIter,*pListIter;
void pushBack(pList A, void* k);
pList CreateList();
void DeleteList(pList A);
pListIter CreateListIter(pList A);
void DeleteListIter(pListIter iter);
void ListIterNext(pListIter* Iterat);
/* return element from ListElment*/
void*  GetIterValue(pListIter iter);
#endif
