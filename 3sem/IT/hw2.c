#include "hw2.h"
#define MSGSZ 128
#define KEY 10



void CreateMessage(int* mspid,key_t key)
{
	int msgflags=IPC_CREAT | 0600;
	int id;
	if ((id = msgget(key, msgflags)) < 0) 
	{
		printf("ERROR IN CREATE\n");
        perror("msgget");
        exit(1);
    }
    else 
     //printf("msgget: msgget succeeded: msqid = %d\n", id);
     
	*mspid=id;
	
}
