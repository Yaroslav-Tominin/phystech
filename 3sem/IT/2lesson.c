#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>













int main(int args,char* argv[])
{
	
	
	
	
	
	
		
	
		
	int i;
	int a;
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
			continue;
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
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	int n;
	int* status;
	int pid;
	int j;
	n=a;
	pid=getpid();
	printf("%d,%d\n",n,pid);
	for(j=0;j<n;++j)
	{
		//printf("KKK");
		if(pid==0)
		{
			printf("GETPID:%d,NUMBER:%d\n",getpid(),j);
			break;
		}
		pid=fork();
		if(pid==0)
		{
			printf("GETPID:%d,NUMBER:%d\n",getpid(),j);
			break;
		}
		wait(NULL);
	}

	
	return 0;
}
