#include "hw2.h"
int main(int args,char * argv[])
{
	int i,k;
	int pid=5;
	key_t key;
	MessageBuf sbuf;
	MessageBuf RecvBuf;
	if((key=ftok("hw2_main.c",1))==-1)
	{
		printf("TROUBLE WITH FTOK\n ");
		exit(EXIT_FAILURE);
	}
	if (args!=2)
	{
		printf("I must have only 2\n ");
		exit(EXIT_FAILURE);
	}
	int* mas=(int*)malloc(sizeof(int));
	read_int(&mas,args,argv);
	int number=mas[0];
	free(mas);
	int mspid;
	CreateMessage(&mspid,key);
	//printf("mspid=%d\n",mspid);
	//printf("CREATED MES QUEUE\n(NEXT CREATE CHILD)\n");
	for(i=1;i<=number;++i)
	{
		
		//printf("Parrents process i=%d\n",i);
		pid=fork();
		if (pid==-1)
		{
			if((msgctl(mspid,IPC_RMID,NULL))<0)
			{
				printf("TROUBLE WITH FORK,ERROR IN DELETE MES QUEUE\n");
				perror("msgdelete");
				exit(1);
			}
			printf("TROUBLE WITH FORK\n ");
			exit(EXIT_FAILURE);
		}
		if (pid==0)
		{
			//start critical section
			if (msgrcv(mspid, &RecvBuf, 0, i, 0) < 0) 
			{
				if((msgctl(mspid,IPC_RMID,NULL))<0)
				{
					printf("TROUBLE WITH RECV,ERROR IN DELETE MES QUEUE\n");
					perror("msgdelete");
					exit(1);
				}	
				perror("msgrcv");
				exit(1);
			}
			
			printf("%d ",i);
			
			fflush(stdout);
			
			
			sbuf.mtype=i+1;
			if (msgsnd(mspid, &sbuf, 0, 0) < 0) 
			{
				if((msgctl(mspid,IPC_RMID,NULL))<0)
				{
					printf("TROUBLE WITH SND,ERROR IN DELETE MES QUEUE\n");
					perror("msgdelete");
					exit(1);
				}
				printf("ERROR IN SEND\n");
				perror("msgsnd");
				exit(1);
			}
			//end critical section
			exit(EXIT_SUCCESS);
			break;
		}
			
	}
	if (pid!=0)
	{
		sbuf.mtype=1;
		if (msgsnd(mspid, &sbuf, 0, 0) < 0) 
		{
			if((msgctl(mspid,IPC_RMID,NULL))<0)
			{
				printf("TROUBLE WITH SEND,ERROR IN DELETE MES QUEUE\n");
				perror("msgdelete");
				exit(1);
			}
			printf("ERROR IN SEND\n");
			perror("msgsnd");
			exit(1);
		}
		
	}
	
	
	
	if (pid!=0)
	{
		//start critical section
		if (msgrcv(mspid, &RecvBuf, 0, number+1, 0) < 0) 
		{
			if((msgctl(mspid,IPC_RMID,NULL))<0)
			{
				printf("TROUBLE WITH RECVV,ERROR IN DELETE MES QUEUE\n");
				perror("msgdelete");
				exit(1);
			}
			perror("msgrcv");
			exit(1);
		}
		//end critical section
		
		//printf("GOOD\n");
		if((msgctl(mspid,IPC_RMID,NULL))<0)
		{
			printf("ERROR IN DELETE MES QUEUE\n");
			perror("msgdelete");
			exit(1);
		}
	}
	return 0;
}
