#include "hw4.h"
int read_char=0;
int total=128;
pid_t pid;
//before fork
void Func_child(int sig)
{
	exit(EXIT_SUCCESS);
}
//before fork
void Func_usr1(int sig)
{
	read_char=read_char+total;
	total=total/2;
	kill(pid,SIGUSR1);
}
//before fork
void Func_usr2(int sig)
{
	total=total/2;
	kill(pid,SIGUSR1);
}
//in child SIG_USR1
void child_empty(int sig)
{
}
// SIGCHILD
void exit_child(int sig)
{
	
}
void trouble_parrent_connection()
{
	exit(EXIT_FAILURE);
}
void Act_SIGCHILD()
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = Func_child;
	sigfillset(&act.sa_mask);
	sigaction(SIGCHLD, &act, NULL);
}
void Act_SIGALRM_child()
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = trouble_parrent_connection;
	//what must be blocked while treat the signal
	sigfillset(&act.sa_mask);
	sigaction(SIGCHLD, &act, NULL);
}
void Act_SIG_USR1()
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = Func_usr1;
	sigfillset(&act.sa_mask);
	sigaction(SIGUSR1, &act, NULL);
}
void Act_SIG_USR1_child()
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = child_empty;
	sigfillset(&act.sa_mask);
	sigaction(SIGUSR1, &act, NULL);
}
void Act_SIG_USR2()
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = Func_usr2;
	sigfillset(&act.sa_mask);
	sigaction(SIGUSR2, &act, NULL);
}
void Block_Sig()
{
	sigset_t signalset;
	sigaddset(&signalset, SIGUSR1);
	sigaddset(&signalset, SIGUSR2);
	sigaddset(&signalset, SIGCHLD);
	sigprocmask(SIG_BLOCK, &signalset, NULL );
	
}
int OpenFile(char* filename)
{
	int descr=open(filename,O_RDONLY);
    if(descr==-1)
	{
		printf("OPEN FILE ERROR\n");
		exit(EXIT_FAILURE);
	}    
	//printf("FILE OPENED\n");
	return descr;
}
int main(int argc,char* argv[])
{
	if (argc != 2) 
    {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(EXIT_FAILURE);
    }
	Act_SIG_USR1();

	Act_SIG_USR2();
	Act_SIGCHILD();

	Block_Sig();
//(3)контекст процесса и контекст прерывания борятся за стек вызовов
	//printf("AFTER INIT SIGHNAL\n");
	int parent_pid=getpid();
	pid=fork();
					//кр. секц. род и ребенок за  маску сигналов
	if (pid==-1)
	{
		printf("TROUBLE WITH FORK\n ");
		exit(EXIT_FAILURE);
	}
	if(pid==0)
	{
				
		sigset_t chl_set;
		sigemptyset(&chl_set);
		Act_SIG_USR1_child();
		Act_SIGALRM_child();
		//printf("AFTER INIT SIGHNAL IN CHILD\n");
		int descr=OpenFile(argv[1]);
		int i;
		char morz;
		//printf("BEFORE\n");
		
		while(read(descr,&morz,1)>0)
		{
			//don't want to wait signal unfinity
			i=128;
			while(i)
			{
				//printf("IN WHILE WHILE CHILD\n");
				if(i&morz)
					kill(parent_pid,SIGUSR1);
				else
					kill(parent_pid,SIGUSR2);
				i=i/2;
				//get signal from parents
				alarm(2);		
		//(2)начало крит секции род и alarm  за маску сигналов	ребенка
				sigsuspend(&chl_set);
			}
				
		}
		////(3)контекст процесса и контекст прерывания борятся за стек
		//(2)конец крит секции род и alarm  за маску сигналов	ребенка
						//конец секции за маску сигналов
		//printf("CHILD END\n");
	}
	if(pid!=0)
	{
		sigset_t par_set;
		
		sigemptyset(&par_set);
		
		while(1)
		{	
			//if (k==53)
				//kill(pid,SIGKILL);
			//printf("IN WHILE PARENTS\n");
			if(total==0)
			{
				
				write(STDOUT_FILENO,&read_char,1);
				total=128;
				read_char=0;
			}
			
			sigsuspend(&par_set);
		//(2)начало крит секции род и alarm  за маску сигналов	ребенка
		}
					
		//printf("PARENT END\n");
						
	}
		//(2)конец крит секции род и alarm  за маску сигналов	ребенка
					//конец секции за таблицу сигналов
//(3)контекст процесса и контекст прерывания борятся за стек вызовов
}
