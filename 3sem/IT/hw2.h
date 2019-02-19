#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include "read_int.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
typedef struct Msgbuf
{
	long mtype;
	int* mtext ;
}MessageBuf,* pMessageBuf;
void CreateMessage(int* mspid,key_t key);

