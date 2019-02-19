#include "read_int.h"
void read_int(int** num,int args,char* argv[])
{
	int i;
	int a;
	int* mas=(int*)malloc(sizeof(int)*(args-1));
	
	for(i=1;i<args;++i)
	{
		char* ptr;
		long ret;
			
		errno=0;
		ret=strtol(argv[i],&ptr,10);
		a=ret;
		if(argv[i]==ptr)
		{
				//printf("BAD\n");
				
		}
		//!strcmp(*(argv[i]),'\0') &&
		else if ( *ptr=='\0' && errno==0)
		{
			mas[i-1]=(int)ret;
			continue;
		}
			//printf("NORM\n%ld",ret);
		else 
		{
			//printf("%ld\n,ptr=%c",ret,*ptr);
			//printf("BaDDDD\n");
			break;
		}
		
		
			//printf("%ld\n",ret);
	}
	*num=mas;
	
}
