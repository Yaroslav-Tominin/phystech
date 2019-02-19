#include "hw3.h"
//V_con
#define BUF_SIZE 500
#define N_SEMBUF 100
void SemVal(int id,char * str)
{
	short semval;
	if(semctl(id,0,GETALL,&semval)<0)
	{
		printf("PROBLEM IN FUNC SEMVAL\n");
		unlink("memory_segment");
		exit(EXIT_FAILURE);
	}
	printf(" %s:%d,VALUE:%d;\n",str,id,semval);
}
void Sem_Val(int id,int num,char * str)
{
	short semval[6];
	if(semctl(id,6,GETALL,semval)<0)
	{
		printf("PROBLEM IN FUNC SEMVAL\n");
		unlink("memory_segment");
		exit(EXIT_FAILURE);
	}
	printf(" %s:%d,VALUE:%d;\n",str,id,semval[num]);
}
short Sem_Val_ret(int id,int num)
{
	short semval[6];
	if(semctl(id,6,GETALL,semval)<0)
	{
		printf("PROBLEM IN FUNC SEMVAL\n");
		unlink("memory_segment");
		exit(EXIT_FAILURE);
	}
	return semval[num];
}

void SegmentDelete(int id)
{
	if(shmctl(id,IPC_RMID,NULL)<0)
	{
		printf("DELETE SEGMENT MEMORY ERROR\n");
		unlink("memory_segment");
		exit(EXIT_FAILURE);
	}    
}
void DeleteSemaphore(int id)
{
	//printf("SEMID IS:%d",id);
	if(semctl(id,0,IPC_RMID)<0)
	{
		printf("PROBLEM DELETE SEMAPHORE\n");
		unlink("semaphore");
		exit(EXIT_FAILURE);
	}
}
int OpenFile(char* filename)
{
	int descr=open(filename,O_RDONLY);
    if(descr==-1)
	{
		printf("OPEN FILE ERROR\n");
		unlink("fifo");
		exit(EXIT_FAILURE);
	}    
	//printf("FILE OPENED\n");
	return descr;
}



void CreateSegmentMemory(int* memory_descr)
{
	int flags=IPC_CREAT|0666;
	int descr;
	key_t key_message;
    if((key_message=ftok("addit1.c",1))==-1)
	{
		printf("TROUBLE WITH FTOK\n ");
		exit(EXIT_FAILURE);
	}
	if((descr=shmget(key_message,sizeof(Message), flags))<0)
	{
		printf("ERROR IN CREATE MESSAGE\n");
        perror("memoryget");
        exit(1);
	}
	//printf("descr:%d\n",descr);
	*memory_descr=descr;
}
void CreateSemBuf(int num,int op,int flgs,struct sembuf* mybuf)
{
	mybuf->sem_num=num;
	mybuf->sem_op=op;
	mybuf->sem_flg=flgs;
}
//
void P_UNDO(int id,int i)
{
	struct sembuf mysembuf;
    CreateSemBuf(i,-1,SEM_UNDO,&mysembuf);
	int k;
    if((k=semop(id,&mysembuf,1))<0)
    {
		//printf("ERROR IN VSem\n");
        //perror("Vsem");
        exit(1);
    }
}
void P(int id,int i)
{
	struct sembuf mysembuf;
    CreateSemBuf(i,-1,0,&mysembuf);
	int k;
    if((k=semop(id,&mysembuf,1))<0)
    {
		printf("ERROR IN VSem\n");
        perror("Vsem");
        exit(1);
    }
}
void V_UNDO(int id,int i)
{
	struct sembuf mysembuf;
	//printf("IN V, id:%d;\n",id);
    CreateSemBuf(i,1,SEM_UNDO,&mysembuf);
	int k;
    if((k=semop(id,&mysembuf,1))<0)
    {
		printf("ERROR IN VSem\n");
        perror("Vsem");
        exit(1);
    }
}

void V(int id,int i)
{
	struct sembuf mysembuf;
	//printf("IN V, id:%d;\n",id);
    CreateSemBuf(i,1,0,&mysembuf);
	int k;
    if((k=semop(id,&mysembuf,1))<0)
    {
		printf("ERROR IN VSem\n");
        perror("Vsem");
        exit(1);
    }
}
void V_con(int id,int i,int j)
{
	struct sembuf mysembuf[3];
	//printf("IN V, id:%d;\n",id);
	CreateSemBuf(j,-1,IPC_NOWAIT,mysembuf);
    CreateSemBuf(j,1,IPC_NOWAIT,&(mysembuf[1]));
    CreateSemBuf(i,1,0,&(mysembuf[2]));
    
	int k;
    if((k=semop(id,mysembuf,3))<0)
    {
		//printf("ERROR IN VSem\n");
        //perror("Vsem");
        exit(1);
    }
}
void P_and_V(int id,int j)
{
	struct sembuf mysembuf[2];
	//printf("IN V, id:%d;\n",id);
	CreateSemBuf(j,-1,0,mysembuf);
    CreateSemBuf(j,1,0,&(mysembuf[1]));
    
    
	int k;
    if((k=semop(id,mysembuf,2))<0)
    {
		printf("ERROR IN VSem\n");
        perror("Vsem");
        exit(1);
    }
}

void P_con(int id,int i,int j)
{
	struct sembuf mysembuf[3];
	//printf("IN V, id:%d;\n",id);
	CreateSemBuf(j,-1,IPC_NOWAIT,mysembuf);
    CreateSemBuf(j,1,IPC_NOWAIT,&(mysembuf[1]));
    CreateSemBuf(i,-1,0,&(mysembuf[2]));
    
	int k;
    if((k=semop(id,mysembuf,3))<0)
    {
		P(id,i);
		//printf("ERROR IN VSem\n");
        //perror("Vsem");
        exit(1);
    }
}
void Waiting(int id,int i,int j)
{
	struct sembuf mysembuf[3];
    CreateSemBuf(i,0,IPC_NOWAIT,mysembuf);
    CreateSemBuf(i,1,SEM_UNDO,&(mysembuf[1]));
    CreateSemBuf(j,0,0,&(mysembuf[2]));
	int k;
    if((k=semop(id,mysembuf,3))<0)
    {
		printf("ERROR IN WAITIN,SERVER HAVE WRITER\n");
        perror("Vsem");
        exit(1);
    }
}
void Waiting_writer(int id,int i,int j)
{
	
	
	
	
	
	struct sembuf mysembuf[5];
	short semval_empty;
	semval_empty=Sem_Val_ret(id,0);
	short semval_full;
	semval_full=Sem_Val_ret(id,1);
	CreateSemBuf(0,semval_empty,0,&(mysembuf[3]));
	CreateSemBuf(1,semval_full,0,&(mysembuf[4]));
	//printf("SEMVAL_EMPTY=%d",semval_empty);
    CreateSemBuf(i,0,IPC_NOWAIT,mysembuf);
    CreateSemBuf(i,1,SEM_UNDO,&(mysembuf[1]));
    CreateSemBuf(j,0,0,&(mysembuf[2]));
	int k;
    if((k=semop(id,mysembuf,5))<0)
    {
		printf("ERROR IN WAITIN,SERVER HAVE WRITER\n");
        perror("Vsem");
        exit(1);
    }
}
void Wait_Sem_write(int id)
{
	struct sembuf mysembuf[2];
    CreateSemBuf(0,0,IPC_NOWAIT,mysembuf);
    CreateSemBuf(0,1,SEM_UNDO,&(mysembuf[1]));
    
	int k;
    if((k=semop(id,mysembuf,2))<0)
    {
		printf("ERROR IN WAITIN,SERVER HAVE WRITER\n");
        perror("Vsem");
        exit(1);
    }
}
void Con_Wait(int id,int i)
{
	struct sembuf mysembuf;
	CreateSemBuf(i,0,0,&mysembuf);
	int k;
    if((k=semop(id,&mysembuf,1))<0)
    {
		printf("ERROR IN WAITIN,SERVER HAVE WRITER\n");
        perror("Vsem");
        exit(1);
    }
}


void Create_Semaphores(int* Semaphores,int num,char* pathname)
{
	int server;
    int flags_server=IPC_CREAT|0666;
    
	key_t key_server;
    if((key_server=ftok(pathname,1))==-1)
	{
		printf("TROUBLE WITH FTOK\n ");
		
		exit(EXIT_FAILURE);
	}
   
   
	if ((server = semget(key_server,num, flags_server)) < 0) 
	{
		
		printf("ERROR IN SEMGET\n");
		DeleteSemaphore(server);
		perror("semget");
		exit(1);
	}
	*Semaphores=server;
}


void CreateSemaphore(int* Semaphore,char* pathname)
{
	int server;
    int flags_server=IPC_CREAT|0600;
    
	key_t key_server;
    if((key_server=ftok(pathname,1))==-1)
	{
		printf("TROUBLE WITH FTOK\n ");
		
		exit(EXIT_FAILURE);
	}
   
   
	if ((server = semget(key_server,1, flags_server)) < 0) 
	{
		
		printf("ERROR IN SEMGET\n");
		DeleteSemaphore(server);
		perror("semget");
		exit(1);
	}
	*Semaphore=server;
}

void Print_Sem(int E_F_S_C_Sc_Cc)
{
	Sem_Val(E_F_S_C_Sc_Cc,0,"EMPTY");
    Sem_Val(E_F_S_C_Sc_Cc,1,"FULL");
    Sem_Val(E_F_S_C_Sc_Cc,2,"SERVER");
    Sem_Val(E_F_S_C_Sc_Cc,3,"CLIENT");
    Sem_Val(E_F_S_C_Sc_Cc,5,"CLIENT_CON");
    Sem_Val(E_F_S_C_Sc_Cc,4,"SERVER_CON");
}
































//Who is opening file and write in separate memory
void Writer(int argc, char *argv[])
{
	if (argc != 2) 
    {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(EXIT_FAILURE);
    }
    int file_descr=OpenFile(argv[1]);
    //printf("WRITER:OPEN FILE\n");
    int memory_descr;
    CreateSegmentMemory(&memory_descr);
    //printf("MEMORY DESCR:%d\n",memory_descr);
    int* adress;
    
    if((adress = (int*)shmat(memory_descr,NULL,0))==(int*)(-1))
	{
		printf("GG\n");
		SegmentDelete(memory_descr);
		printf("PROBLEM WITH SHMAT\n ");
		exit(EXIT_FAILURE);
	}
    //adress is 0 if haven't information
    //Create 1 semaphore Semget
    pMessage m=(pMessage)((void*)adress);
    //printf("WRITER:CREATE MEMORY\n");
    
    int E_F_S_C_Sc_Cc;
    Create_Semaphores(&E_F_S_C_Sc_Cc,6,"Client_connection.c");
    //SemVal(Server,"SERVER");
    //printf("WRITER:CREATE SERVER SEMAPHORE\nNEXT WAITING\n");
    //not writer who do critical section
    int Sem_wr;
    Create_Semaphores(&Sem_wr,1,"Server_connection.c");
									
    Wait_Sem_write(Sem_wr);
				//(1)начало крит. секции writer'ы борятся за память memory	
				//(2) начало кр.сек. wr и rd борятся за full empty		
    //Print_Sem(E_F_S_C_Sc_Cc);
    //no reader,who have already being worked
	
    Waiting_writer(E_F_S_C_Sc_Cc,2,4);
				//(2) конец кр.сек. wr и rd борятся за full empty
    //printf("WRITER:NEXT WAIT CLIENT\n");
    //wait client
    P_and_V(E_F_S_C_Sc_Cc,3);
								
    V_UNDO(E_F_S_C_Sc_Cc,5);
    //SemVal(Server,"SERVER");
    //printf("WRITER:AFTER WAITING ;NEXT CREATE EMPTY FULL\n");
    //have only 1 writer
    int t;
    
    
    
    V(E_F_S_C_Sc_Cc,0);
    
    
    
    
    //Print_Sem(E_F_S_C_Sc_Cc);
    do
    {
		//printf("WRITER:NNEXT P(EMPTY)\n");
		P_con(E_F_S_C_Sc_Cc,0,3);
						//(3)начало кр. секц. writer, reader  за memory
		//Sem_Val(E_F_S_C,0,"EMPTY");
		//printf("WRITER: AFTER P(EMPTY; NEXT READ,V(FULL)\n");
		Message mes;
		t=read(file_descr,mes.mes,BUF_SIZE);
		mes.size=t;
		*m=mes;
						//(3)конец кр. секц. writer, reader  за memory
		V_con(E_F_S_C_Sc_Cc,1,3);
									
		//Sem_Val(E_F_S_C,1,"FULL");
		//printf("WRITER: AFTER V(FULL)\n");
	}while(t!=0);
	
	
	P_UNDO(E_F_S_C_Sc_Cc,5);
				//(1)конец крит. секции writer'ы борятся за память memory	
    P_UNDO(E_F_S_C_Sc_Cc,2);
								
	//printf("END WRITER\n");
}









































void Reader()
{
    int memory_descr;
    CreateSegmentMemory(&memory_descr);
    //printf("READER: CREATE MEMORY\n");
    int* adress;
    if((adress=(int*)shmat(memory_descr,NULL,0))==(int*)-1)
	{
		printf("PROBLEM WITH SHMAT\n ");
		exit(EXIT_FAILURE);
	}
    //adress is 0 if haven't information
    //Create 1 semaphore Semget
    pMessage m=(pMessage)(void*)adress;
    
    
    int E_F_S_C_Sc_Cc;
    Create_Semaphores(&E_F_S_C_Sc_Cc,6,"Client_connection.c");
    //printf("READER: CREATE CLIENT SEMAPHORE\nNEXT WAITING\n");
    //SemVal(Client,"CLIENT");
    //Print_Sem(E_F_S_C_Sc_Cc);
						
    Waiting(E_F_S_C_Sc_Cc,3,5);
    //have not old writer	
				//(1)начало крит. секции reader'ы борятся за память memory
				//(2)начало крит. секции rd,wr борятся за F,E
    int t;
    //Sem_Val(E_F_S_C,0,"EMPTY");
    //Sem_Val(E_F_S_C,1,"FULL");
				//(2)конец крит. секции rd,wr борятся за F,E
    P_and_V(E_F_S_C_Sc_Cc,2);
				
    V_UNDO(E_F_S_C_Sc_Cc,4);
    //printf("EE\n");
    //P_and_V(E_F_S_C,2);
    
    
    //Print_Sem(E_F_S_C_Sc_Cc);
    
    do
    {
		//Sem_Val(E_F_S_C,1,"FULL");
		//Sem_Val(E_F_S_C,0,"EMPTY");
		
		//printf("READER: NEXT CREATE P(FULL)\n");
		
		P_con(E_F_S_C_Sc_Cc,1,2);
					//(3)начало крит. секции rd,wr борятся за memory
		//Sem_Val(E_F_S_C,1,"FULL");
		Message mes;
		mes=*m;
		t=write(STDOUT_FILENO,mes.mes,mes.size);
		//printf("READER: NEXT CREATE V(EMPTY)\n");
					//(3)конец крит. секции rd,wr борятся за memory
		V_con(E_F_S_C_Sc_Cc,0,2);
		//Sem_Val(E_F_S_C,0,"EMPTY");
		//printf("READER:HERE\n");
	}while(t!=0);
	P(E_F_S_C_Sc_Cc,0);
	P_UNDO(E_F_S_C_Sc_Cc,4);
			//(1)конец крит. секции reader'ы борятся за память memory
    P_UNDO(E_F_S_C_Sc_Cc,3);
					
    //SemVal(Client,"CLIENT");
    //Sem_Val(E_F_S_C,0,"EMPTY");
    //Sem_Val(E_F_S_C,1,"FULL");
    
    //SemVal(EMPTY,"EMPTY");
    //printf("READER<THE END\n");
    //printf("END WRITER\n");
    
    
		
}
    
