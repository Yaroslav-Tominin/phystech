#ifndef __parc__
#define __parc__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
typedef enum JsonElementValueType
{
	JSON_VALUE,
	JSON_MAP,
	JSON_ARRAY
}JsonElementValueType;
typedef struct JsonElement
{
	JsonElementValueType type;
	char *key;
	void * value;
}JsonElement, * pJsonElement;
void* ParsingMap(char** cur);
void* ParsingValue( char** cur);
void* ParsingArray(char** cur);
void DeleteMapParsing(pHash h);
void DeleteArrayParsing(pList list);
#endif
