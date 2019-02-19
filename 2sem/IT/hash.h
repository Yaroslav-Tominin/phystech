#ifndef __hash__
#define __hash__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


typedef struct HashTable
{
	pList* table;
	int size;
} Hash,* pHash;


typedef struct Map_element
{
	char* key;
	void* value;
}MapElement, * pMapElement;


pHash CreateHash(int size);
int HashFunction( char* a);
void DeleteHash(pHash* hash);



void AddToMap(pHash h,char* key,void* value);
int IsInMap(pHash h,char* key);
void* GetFromMap(pHash h,char* key);
void* GiveKeyFromMap(pHash h);

#endif
