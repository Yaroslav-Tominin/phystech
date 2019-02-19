#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "read_int.h"
int main(int args,char* argv[])
{
	int* mas=(int*)malloc(sizeof(int)*(args-1));
	read_int(&mas,args,argv);
	int n=mas[0];
	int k=mas[1];
	int i;
	int sum;
	int pid;
	//we have n process and incremeting k
	printf("N=%d,K=%d\n",n,k);
	for (i=0;i<n;++i)
	{
		if(pid==0)
		{
			int j;
			for(j=0;j<k;++j)
			{
				++sum;
				printf("%d\n",sum);
			}
			
			break;
		}
		pid=fork();
	}
	if(pid!=0)
	{
		for(i=0;i<n;++i)
		{
			wait(NULL);
		}
	}
	
}
