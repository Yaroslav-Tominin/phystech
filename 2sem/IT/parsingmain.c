#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsing.h"
int main()
{

	FILE * f1=fopen("parsingfile.txt","rt");
	fseek(f1,0,SEEK_END);
	unsigned size=ftell(f1);
	fseek(f1,0,SEEK_SET);
	char* Text=(char*)malloc(sizeof(char)*size);
	fread(Text,sizeof(char),size,f1);



	//printf("Pars:%d\n\n\n",Pars(Text,h));
	//printf("IsInMap:%d\n",IsInMap(h,"key1"));
	//printf("IsInMap:%d\n",IsInMap(h,"key2"));
	//printf("%d\n",IsInMap(h,"key2"));
	//printf("%d\n",IsInMap(h,"key3"));
	//printf("%d\n",IsInMap(h,"key4"));
	//printf("%s\n",GetFromMap(h,"key4"));
	//TrueParsKeyAndValue(Text,1);

	char* current0=Text;
    char** cur0=&current0;
	void* h=ParsingMap(&Text);

	if(h==NULL)
		printf("THIS IS NULL\n");
    else
    {
        printf("\n\n\n%d\n",IsInMap(h,"key\"1"));
        printf("%d\n",IsInMap(h,"key2"));
        printf("%d\n",IsInMap(h,"key3"));
        printf("%d\n",IsInMap(h,"key4"));
        printf("%d\n",IsInMap(h,"key5"));
        printf("%d\n",IsInMap(h,"key6"));
        printf("%d\n",IsInMap(h,"key7"));
	printf("%d\n",IsInMap(h,"key8"));

        //printf("%s\n",((pJsonElement)GetFromMap(h,"ke\\\"y1"))->value);
        printf("key:%s, value: %s\n",((pJsonElement)GetFromMap(h,"key2"))->key,(char*)(((pJsonElement)GetFromMap(h,"key2"))->value));
        printf("key:%s, value: %s\n",((pJsonElement)GetFromMap(h,"key3"))->key,(char*)(((pJsonElement)GetFromMap(h,"key3"))->value));
        pHash g=(pHash)(((pJsonElement)GetFromMap(h,"key44"))->value);
        printf("key:%s, value: %s\n",((pJsonElement)GetFromMap(g,"key22222"))->key,(char*)(((pJsonElement)GetFromMap(g,"key22222"))->value));
        printf("key:%s, value: %s\n",((pJsonElement)GetFromMap(h,"key5"))->key,(char*)(((pJsonElement)GetFromMap(h,"key5"))->value));
        printf("key:%s, value: %s\n",((pJsonElement)GetFromMap(h,"key6"))->key,(char*)(((pJsonElement)GetFromMap(h,"key6"))->value));
        printf("key:%s, value: %s\n",((pJsonElement)GetFromMap(h,"key7"))->key,(char*)(((pJsonElement)GetFromMap(h,"key7"))->value));
        pJsonElement jsn=((pJsonElement)GetFromMap(h,"key8"));
        if(jsn->type==JSON_ARRAY)
            printf("ARRAY\n");

        pList list=(pList) jsn->value;
        pListIter iter = CreateListIter(list);
        pJsonElement e=(pJsonElement) GetIterValue(iter);

        printf("%s\n",(char*)(e->value));
        pList keys=(pList)(GiveKeyFromMap(h));
        pListIter iter1 = CreateListIter(keys);
        printf("1:%s\n",(char*)GetIterValue(iter1));
        ListIterNext(&iter1);
        printf("2:%s\n",(char*)GetIterValue(iter1));
        DeleteListIter(iter);
        DeleteListIter(iter1);
        DeleteList(keys);
    }
    DeleteMapParsing(h);
    free((void*)current0);
    fclose(f1);


	return 0;
}
