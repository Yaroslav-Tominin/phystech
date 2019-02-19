#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int args,char* argv[])
{
	
	char* nargv[args-1];
	int i;
	for(i=2;i<args;++i)
	{
		nargv[i-2]=argv[i];
	}
	nargv[args-1]=(char*) 0;
	printf("this is the original program\n");
	printf("%s\n",argv[1]);
	
	execv(argv[1], nargv);
	printf("this is the original program\n");
	return 0;
}
