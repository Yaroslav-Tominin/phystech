#include "hw1.h"
#define FIFO_TOGETHER "TogetherFifo"
#define SEPARATE_FIFO "fifo"
#include <errno.h>
#include <unistd.h>
void CreateFifoTogether(char* fname)
{
	if (access(fname, F_OK) == -1) 
    {
		int res = mkfifo(fname, 0666);
		if (res == -1)
		{
			
			fprintf(stderr, "Gould not create fifo %sn", "fifo");
			exit(EXIT_FAILURE);
		}
		//printf("Fifo was created\n");
	}
	//printf("Fifo was created before\n");
}








char* CreateFifoSeparate(pid_t pid)
{
	char* fname= (char*) malloc(sizeof(char)*100);
	int a=snprintf(fname,99,"%s%d",SEPARATE_FIFO,pid);
	if(a<0 || a>99)
	{
		printf("Gould not create separate fifo ");
		free(fname);
		exit(EXIT_FAILURE);
	}
	CreateFifoTogether(fname);
	return fname;
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







































//Who is opening file and writing in SEPARATE_FIFO
void Reader(int argc, char *argv[])
{
	ssize_t t;
    int n=150;
    char* buf=(char*)malloc(sizeof(char)*200);
    int fd;
    int file;
    if (argc != 2) 
    {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		free(buf);
		exit(EXIT_FAILURE);
    }
    CreateFifoTogether(FIFO_TOGETHER);
	file=OpenFile(argv[1]);
	
	
	//printf("Create fifo TOG and open FILE(RD)\n(NEXT OPEN FIFO_TOG on RD)\n\n");
	
	
	
	
	
	//we want to get the pid of reading process from FIFO_TOGETHER
	
	int fifo_r=open(FIFO_TOGETHER,O_RDONLY);
	
	
	
	if(fifo_r==-1)
	{
		printf("PROBLEM WITH READER,OPEN FIFO_TOGETHER\n\n");
		free(buf);
		exit(EXIT_FAILURE);
		
	}
	//printf("OPEN FIFO_TOG on RD\n(NEXT Read fifo_TOG)\n\n");
	//critical section
	pid_t fifo_pid;
	t=read(fifo_r,&fifo_pid,sizeof(pid_t));
	//end critical section
	if (t!=sizeof(pid_t))
	{
		printf("PROBLEM WITH READER,READ FIFO_TOGETHER\n\n");
		free(buf);
		exit(EXIT_FAILURE);
	}
	//printf("Read fifo_TOG\n(NEXT CLOSE FIFO_TOG )\n\n");
	int fifo_tog_close=close(fifo_r);
	if(fifo_tog_close==-1)
	{
		printf("PROBLEM WITH READER,CLOSE FIFO_TOGETHER\n\n");
		free(buf);
		
		exit(EXIT_FAILURE);
	}
	
	
	//printf("CLOSE FIFO_TOG \n(Opened FIFO_SEPAR ON WR)\n\n");
	char* fname=CreateFifoSeparate(fifo_pid);
	//open FIfo separate on write
	
	int fifo_s=open(fname,O_WRONLY|O_NONBLOCK);
	if(fifo_s==-1)
	{
		printf("PROBLEM WITH READER,OPEN FIFO_SEP\n\n");
		free(buf);
		free(fname);
		exit(EXIT_FAILURE);
	}
	int fcn;
	fcn = fcntl(fifo_s, F_GETFL, 0);
	fcntl(fifo_s, F_SETFL, fcn & ~O_NONBLOCK);
	
	//printf("Opened FIFO_SEPAR ON WR\n(WRITED IN FIFO_SEP)\n\n");
	
	
	t=read(file, buf, n);
	while(t>0)
	{
		write(fifo_s, buf, t);
		t=read(file, buf, n);
	}
	int c=close(file);
	if(c==-1)
	{
		printf("PROBLEM WITH CLOSINH FILE\n");
		free(buf);
		free(fname);
		exit(EXIT_FAILURE);
	}
	//printf("WRITED IN FIFO_SEP\n\n");
	int fifo_sep_close=close(fifo_s);
	if(fifo_sep_close==-1)
	{
		printf("PROBLEM WITH READER,CLOSE FIFO_SEPARAYE\n");
		free(buf);
		free(fname);
		exit(EXIT_FAILURE);
	}
	
	free(buf);
	free(fname);
}

/*It create a separate fifo and write pid in FIFO_TOGETHER,
 after this it listen the separate fifo
*/




























void Writer()
{
	ssize_t t;
    int n=150;
    char* buf=(char*)malloc(sizeof(char)*200);
	char* fifo_separate;
	pid_t pid=getpid();
	fifo_separate=CreateFifoSeparate(pid);
	CreateFifoTogether(FIFO_TOGETHER);
	//printf("CREATE TWO FIFO\n (NEXT OPEN FIFO_SEP ON READ|NONBLOCK)\n");
	//don't important (if kill writer, reader not kill)
	int k_sep=open(fifo_separate,O_RDONLY|O_NONBLOCK);
	if(k_sep==-1)
	{
		printf("PROBLEM WITH WRITER,OPEN FIFO_SEPARATE111\n");
		free(buf);
		free(fifo_separate);
		exit(EXIT_FAILURE);
	}
	//printf("OPEN FIFO_SEP ON READ|NONBLOCK\n(NEXT OPEN FIFO_SEP ON WR)\n");
	//for read in sep don't return 0,if write in sep doesn't exist
	//we must close it when read get the first inform
	int k_separate_wr=open(fifo_separate,O_WRONLY|O_NONBLOCK);
	if(k_separate_wr==-1)
	{
		printf("PROBLEM WITH WRITER,OPEN FIFO_TOGETHER\n");
		free(buf);
		free(fifo_separate);
		exit(EXIT_FAILURE);
	}
	//printf("OPEN FIFO_SEP ON WR|NONBLOCK\n(NEXT CHANHE NONBLOCK IN FIFO_SEP)\n");
	
	
	
	
	
	
	//printf(" CHANHE NONBLOCK IN FIFO_SEP\n(NEXT OPEN FIF_TOGETHER WR)\n");
	int k=open(FIFO_TOGETHER,O_WRONLY);
	if(k==-1)
	{
		printf("PROBLEM WITH WRITER,OPEN FIFO_TOGETHER\n");
		free(buf);
		free(fifo_separate);
		exit(EXIT_FAILURE);
	}
	//printf("OPEN FIF_TOGETHER WR\n(NEXT WRITE IN FIFO_TOGETHER pid)\n");
	
	
	
	
	
	t=write(k,&pid,sizeof(pid_t));
	if (t!=sizeof(pid_t))
	{
		printf("PROBLEM WITH WRITER,WRITE IN FIFO\n");
		free(buf);
		free(fifo_separate);
		exit(EXIT_FAILURE);
	}
	
	//printf("WRITE IN FIFO_TOGETHER pid\n()\n");
	
	
	
	//critical section
	t=read(k_sep, buf, n);
	int i=0;
	int z=2;
	while(i<20)
	{
		usleep(z);
		t=read(k_sep, buf, n);
		
		if (t!=-1)
		{
			i=20;
		}
		
		++i;
		z=2*z;
	}
	if (t==-1)
	{
		printf("PROBLEM WITH WRITER,PPPPPP\n");
		printf("ERRNO:%d",errno);
		free(buf);
		free(fifo_separate);
		exit(EXIT_FAILURE);
	}
	//printf("(NEXT CLOSE FIFO_SEP WRITE)\n");

	
	
	
	int fcn;
	fcn = fcntl(k_sep, F_GETFL, 0);
	fcntl(k_sep, F_SETFL, fcn & ~O_NONBLOCK);
	
	
	
	
	int k_sep_wr=close(k_separate_wr);
	if(k_sep_wr==-1)
	{
		printf("PROBLEM WITH WRITER,CLOSE FIFO_TOGETHER\n");
		printf("ERRNO:%d",errno);
		free(buf);
		free(fifo_separate);
		exit(EXIT_FAILURE);
	}
	
	while(t>0)
	{
		write(STDOUT_FILENO, buf, t);
		t=read(k_sep, buf, n);
	}
	//printf("RED IN FIFO_SEP\n");
	
	
	
	//end critical section
	int fifo_tog_close=close(k);
	
	if(fifo_tog_close==-1)
	{
		printf("PROBLEM WITH WRITER,CLOSE FIFO_TOGETHER\n");
		printf("ERRNO:%d",errno);
		free(buf);
		free(fifo_separate);
		exit(EXIT_FAILURE);
	}
	
	
	
	int c=close(k_sep);
	
	if(c==-1)
	{
		printf("PROBLEM WITH CLOSING FIFO_SEPARATE\n");
		free(buf);
		free(fifo_separate);
		exit(EXIT_FAILURE);
	}
	
	
	free(buf);
	free(fifo_separate);
}
