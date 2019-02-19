#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "read_int.h"
void* K_iterator(void* ful)
{
	
	int i;
	int* sum;
	int* k;
	k=((int**)ful)[0];
	
	sum=((int**)ful)[1];
	printf("IN K_iterator,sum=%d\n\n\n\n",*sum);
	for(i=0;i<*k;++i)
	{ 
		++(*sum);
	}
	return NULL;
}
int main(int args,char* argv[])
{
	//int* mas=(int*)malloc(sizeof(int)*(args-1));
	int* mas;
	read_int(&mas,args,argv);
	int n=mas[0];
	int k=mas[1];
	int i;
	int sum=0;
	int pid;
	int** ar=(int**)malloc(sizeof(int*)*2);
	ar[0]=&k;
	ar[1]=&sum;
	//we have n process and incremeting k
	printf("N=%d,K=%d\n",n,k);
	pthread_t thread;
	pthread_t thread2;
	for (i=0;i<n;++i)
	{	
		pthread_create(&thread,NULL,K_iterator,(void*)ar);
		pthread_join(thread,NULL);
	}
	printf("SUM IS %d\n",sum);
	
	free(ar);
	free(mas);
	
	return 0;
	
}
