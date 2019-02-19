#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include "read_int.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#define BUF_SIZE 500










typedef struct message
{
	size_t size;
	char mes[BUF_SIZE];
	
}Message,*pMessage;













void SemVal(int id,char * str);
void Sem_Val(int id,int num,char * str);
void SegmentDelete(int id);
void DeleteSemaphore(int id);
int OpenFile(char* filename);
void CreateSegmentMemory(int* memory_descr);
void CreateSemBuf(int num,int op,int flgs,struct sembuf* mybuf);
void P_UNDO(int id,int i);
void P(int id,int i);
void V_UNDO(int id,int i);
void V(int id,int i);
void V_con(int id,int i,int j);
void P_and_V(int id,int j);
void P_con(int id,int i,int j);
void Waiting(int id,int i,int j);
void Con_Wait(int id,int i);
void Create_Semaphores(int* Semaphores,int num,char* pathname);
void CreateSemaphore(int* Semaphore,char* pathname);
void Print_Sem(int E_F_S_C_Sc_Cc);

























































void Writer(int argc, char *argv[]);
void Reader();
