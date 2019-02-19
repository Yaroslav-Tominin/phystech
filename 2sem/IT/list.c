#include "list.h"
void pushBack(pList A, void* k)
{
	pListElement e=(pListElement)malloc(sizeof(ListElement));
	e->element=k;
	if(A->start)
	{
		A->end->next=e;
		e->prev=A->end;
		A->end=e;
		e->next=NULL;
	}
	else
	{
		A->start=e;
		A->end=e;
		e->prev=NULL;
		e->next=NULL;
	}
}
pList CreateList()
{
	pList A=(pList)malloc(sizeof(List));
	A->start=NULL;
	A->end=NULL;
	return A;
}
void DeleteList(pList A)
{
	pListIter iter=CreateListIter(A);
	while(iter)
	{
		free(iter->value);
		ListIterNext(&iter);
	}
	free(A);
}

pListIter CreateListIter(pList A)
{
	if(A->start == NULL)
		return NULL;
	pListIter iter=(pListIter)malloc(sizeof(ListIter));
	iter->value=A->start;
	if(A->start->next)
	{
		iter->next=A->start->next;
		return iter;
	}
	iter->next=NULL;
	return iter;
}
void DeleteListIter(pListIter iter)
{
	free(iter);
}
void ListIterNext(pListIter* Iterat)
{
	pListIter iter=*Iterat;
	iter->value=iter->next;
	if(iter->next!=NULL)
	{
		iter->next=iter->next->next;
		return;
	}
	else 
	{
		free(iter);
		*Iterat=NULL;
	}
}
void* GetIterValue(pListIter iter)
{
	if(iter==NULL)
		return NULL;
	return iter->value->element;
}
