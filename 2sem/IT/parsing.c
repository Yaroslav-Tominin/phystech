#include "parsing.h"

#include <string.h>

void RunParsing(char** cur)
{
    char* current=*cur;
	while(*current==' ' || *current=='\t'|| *current=='\n'|| *current=='\r'|| *current=='\\')
	{
		++current;
	}
	*cur=current;
}
/*run after quotation ret after quotation*/

char* ParsBetween(char**cur)
{
    char* current=*cur;

	while(*current!='"')
	{
        if(*current=='\\')
            current=current+2;
        else
            ++current;
	}

	char* par=(char*)malloc((current-*cur+1)*sizeof(char));
	int size=(current-*cur)/sizeof(char);
	par[size]='\0';
	int i;
	for (i=0;i<size;++i)
	{
		par[i]=*(*cur+i);
	}
	++current;
    *cur=current;
	return par;
}
/*run before quotation ret after :*/
void* ParsKey(char** cur)
{
	char* current=*cur;
	RunParsing(&current);
	if(*current!='"')
        return NULL;
    ++current;
	char* key=ParsBetween(&current);
	RunParsing(&current);
	if(*current!=':')
	{
		free(key);
		return NULL;
	}
	else
		current+=1;
	RunParsing(&current);
	*cur=current;
	return key;
}
void* ParsingMap(char** cur)
{

	char* current=*cur;
	pHash h=CreateHash(20);
	if(*current!='{')
		return NULL;
	++current;
	while(*current!='}')
	{
		void*k=ParsKey(&current);
		if(k==NULL)
		{
			return NULL;
		}
		char* key=k;
		void* j=ParsingValue(&current);

		if(j==NULL)
		{
			return NULL;
		}
	    ((pJsonElement)j)->key=key;
		AddToMap(h,key,j);
        RunParsing(&current);

	}
	/* miss bracket*/
	++current;
	*cur=current;
	printf("AA\n");
	return (void*)h;
}
void* ParsingValue( char** cur)
{
	char* current=*cur;
	RunParsing(&current);
	if(*current=='"')
	{
	    ++current;
		char* value=ParsBetween(&current);
		RunParsing(&current);
		if(*current!=',')
		{
			return NULL;
		}
		++current;
		RunParsing(&current);
		*cur=current;
		pJsonElement json=(pJsonElement)malloc(sizeof(JsonElement));
		json->type=JSON_VALUE;
		json->value=value;
		return json;
	}
	else if(*current=='[')
    {
        
        void* value=ParsingArray(&current);
        if(value==NULL)
        {
            
            return NULL;
        }
        pJsonElement json=(pJsonElement)malloc(sizeof(JsonElement));
        json->type=JSON_ARRAY;
        json->value=value;
        if(*current!=',')
		{
			free(value);
			return NULL;
		}
		++current;
		RunParsing(&current);
        *cur=current;
            
        return json;
    }
	else if (*current!='"' && *current!='{')
	{
		return NULL;
	}
	void* value=ParsingMap(&current);

	if(value==NULL)
        return NULL;
    if(*current!=',')
		{
			free(value);
			return NULL;
		}
		++current;
		RunParsing(&current);
	pJsonElement json=(pJsonElement)malloc(sizeof(JsonElement));
    json->type=JSON_MAP;
    json->value=value;
	*cur=current;
    return json;

}
void* ParsingArray(char** cur)
{
    
	char* current=*cur;
	
	pList array=CreateList();
	if(*current!='[')
    {
        
		return NULL;
    }
	++current;
	while(*current!=']')
	{
	    RunParsing(&current);
	    
	    void* j=ParsingValue(&current);
		printf("ARRAY:%s\n",(char*)j);
		if(j==NULL)
		{
		    
			return NULL;
		}
	    ((pJsonElement)j)->key=NULL;
	    pushBack(array,j);
        RunParsing(&current);
	}
	/*miss bracket*/
	++current;
	*cur=current;
	

	return (void*)array;
}




void DeleteMapParsing(pHash h)
{
    pList keys=(pList)GiveKeyFromMap(h);
    pListIter iter=CreateListIter(keys);
    while (iter)
    {
        pJsonElement json=((pJsonElement)GetFromMap(h,(char*)(GetIterValue(iter))));
        if (json->type==JSON_MAP)
        {
            DeleteMapParsing(json->value);
            free(json->key);
            free(json);
        }
        else if(json->type==JSON_ARRAY)
        {
            DeleteArrayParsing(json->value);
            free(json);
            free((char*)(GetIterValue(iter)));
        }
        else if(json->type==JSON_VALUE)
        {
			free(json->key);
			free(json->value);
            free(json);
        }
        ListIterNext(&iter);
    }
    DeleteHash(&h);
    DeleteListIter(iter);
    DeleteList(keys);
}
void DeleteArrayParsing(pList list)
{
    pListIter iter=CreateListIter(list);
    while (iter)
    {
        pJsonElement json=(pJsonElement)GetIterValue(iter);
        if (json->type==JSON_MAP)
        {
            DeleteMapParsing(json->value);
            free(json->key);
            free(json);

        }
        else if(json->type==JSON_ARRAY)
        {
            DeleteArrayParsing(json->value);
            free(json);
        }
        else if(json->type==JSON_VALUE)
        {
			free(json->key);
			free(json->value);
            free(json);
        }
        ListIterNext(&iter);
    }
    DeleteList(list);
}
