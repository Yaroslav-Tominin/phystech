#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
int main(int args,char* argv[])
{
	int i;
	for(i=1;i<args;++i)
	{
		char* ptr;
		long ret;
		errno=0;
		ret=strtol(argv[i],&ptr,10);
		if(argv[i]==ptr)
		{
			//printf("BAD\n");
			
		}
		//!strcmp(*(argv[i]),'\0') &&
		else if ( *ptr=='\0' && errno==0)
			//printf("NORM\n%ld",ret);
		else 
		{
			//printf("%ld\n,ptr=%c",ret,*ptr);
			//printf("BaDDDD\n");
			break;
		}
		
		//printf("%ld\n",ret);
	}
		
	
		
	//printf("%d",args);
	return 0;
}