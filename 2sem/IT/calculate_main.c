#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsing.h"
#include "calculate.h"
int main()
{

	FILE * f1=fopen("calculate.txt","rt");
	fseek(f1,0,SEEK_END);
	unsigned size=ftell(f1);
	fseek(f1,0,SEEK_SET);
	char* Text=(char*)malloc(sizeof(char)*size);
	fread(Text,sizeof(char),size,f1);
	char *t = Text;
	void* h=ParsingMap(&Text);
	if(h==NULL)
		printf("THIS IS NULL\n");
	float f=ParsingCalculate(h);
	printf("a=%f",f);
	DeleteMapParsing(h);
	free(t);
	fclose(f1);
	return 0;
}
