  #include "hash.h"
pHash CreateHash(int size)
{
	int i;
	pHash hash=(pHash)malloc(sizeof(Hash));
	pList* table=(pList*)malloc(sizeof(pList)*size);
	for(i=0;i<size;++i)
		table[i]=CreateList();
	hash->table=table;
	hash->size=size;
	return hash;
}
int HashFunction( char* a)
{
	int hash=7;
	int i;
	for(i=0;i<strlen(a);++i)
		hash=hash*31+a[i];
	return hash;
}

void AddToMap(pHash h,char* key,void* value)
{
	pMapElement elem=(pMapElement)malloc(sizeof(MapElement));
	elem->key=key;
	elem->value=value;
	int f=HashFunction(key);
	pushBack((h->table)[f%(h->size)],(void*)elem);
}
int IsInMap(pHash h,char* key)
{
	int f=HashFunction(key);
	pList Lst=(h->table)[f%(h->size)];
	pListIter iter=CreateListIter(Lst);
	while(iter)
	{
		char* ckey =((pMapElement)GetIterValue(iter))->key;
		if(strcmp(ckey,key)==0)
		{
			DeleteListIter(iter);
			return 1;
		}
		else
			ListIterNext(&iter);
	}
	DeleteListIter(iter);
	return 0;
}
void* GetFromMap(pHash h,char* key)
{
	int f=HashFunction(key);
	pList Lst=(h->table)[f%(h->size)];
	pMapElement e1;
	pListIter iter=CreateListIter(Lst);
	while(iter)
	{
		char* ckey =((pMapElement)GetIterValue(iter))->key;
		if(strcmp(ckey,key)==0)
		{
			e1=((pMapElement)GetIterValue(iter))->value;
			DeleteListIter(iter);
			return e1;
		}
		else
			ListIterNext(&iter);
	}
	DeleteListIter(iter);
	return NULL;
}
void* GiveKeyFromMap(pHash h)
{
    pList Lst;
    pList keys=CreateList();
	int i;
	for(i=0;i<(h->size);++i)
	{
		Lst=(h->table)[i];
		pListIter iter=CreateListIter(Lst);
		while(iter)
		{
		    pushBack(keys,((pMapElement)GetIterValue(iter))->key);
			ListIterNext(&iter);

		}
	}
	return keys;
}
void DeleteHash(pHash* h)
{
	pHash hash=*h;
	pList Lst;
	int i;
	for(i=0;i<(hash->size);++i)
	{
		Lst=(hash->table)[i];
		pListIter iter=CreateListIter(Lst);
		while(iter)
		{
			free((pMapElement)(GetIterValue(iter)));
			ListIterNext(&iter);

		}
        DeleteListIter(iter);
		DeleteList(Lst);
	}
	free(hash->table);
	free(hash);
	h=NULL;
}

