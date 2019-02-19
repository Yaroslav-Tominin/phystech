#include "hw5.h"
#include <sys/time.h>
#define r 0
#define w 1

typedef struct children
{
	int pipefd_P_to_CHLD[2];
	int pipefd_CHLD_to_P[2];
	int size_buf;
	int first_in_buf;
	int quant;
	char* child_buf;
}Children,*pChildren;
void ParentsCloseDescr(pChildren ChildArray,int n)
{
	int i;
	for(i=0;i<n;++i)
	{
		if(i==0)
		{
			close(ChildArray[i].pipefd_P_to_CHLD[r]);
			close(ChildArray[i].pipefd_CHLD_to_P[w]);
		}
		
		else
		{
			close(ChildArray[i].pipefd_P_to_CHLD[r]);
			close(ChildArray[i].pipefd_CHLD_to_P[w]);
		}
	}
	
}
void ChildCloseDescr(pChildren ChildArray , int n,int i)
{
	int j;
	for(j=0;j<n;++j)
	{
		if(i!=j && j==0)
		{
			close(ChildArray[j].pipefd_P_to_CHLD[r]);
			close(ChildArray[j].pipefd_CHLD_to_P[0]);
			close(ChildArray[j].pipefd_CHLD_to_P[1]);
		}
		else if(i!=j)
		{
			close((ChildArray[j]).pipefd_P_to_CHLD[0]);
			close((ChildArray[j]).pipefd_P_to_CHLD[1]);
			close((ChildArray[j]).pipefd_CHLD_to_P[0]);
			close((ChildArray[j]).pipefd_CHLD_to_P[1]);
		}
		else if(j==0)
		{
			close(ChildArray[j].pipefd_CHLD_to_P[r]);
		}
		
		else
		{
			close(ChildArray[j].pipefd_P_to_CHLD[w]);
			close(ChildArray[j].pipefd_CHLD_to_P[r]);
		}
	}
}
void PrintSet(pChildren ChildArray,fd_set R,fd_set W,int n)
{
	int ww,rr,i;
	for(i=0;i<n;++i)
	{
		rr=(ChildArray[i]).pipefd_CHLD_to_P[r];
		ww=(ChildArray[i]).pipefd_P_to_CHLD[w];
		if(FD_ISSET(rr,&R))
		{
			printf("\n\nBATYA,rr=%d\n",rr);
		}
		if(FD_ISSET(ww,&W))
		{
			printf("\n\nBATYA,ww=%d\n",ww);
		}
	}
}
void PrintArrayChld(pChildren ChildArray,int n)
{
	int a,b,c,d;
	int i;
	for (i=0;i<n;++i)
	{
		a=((ChildArray[i]).pipefd_P_to_CHLD)[0];
		b=((ChildArray[i]).pipefd_P_to_CHLD)[1];
		c=((ChildArray[i]).pipefd_CHLD_to_P)[0];
		d=((ChildArray[i]).pipefd_CHLD_to_P)[1];
		printf("IN CHILD =%d,P_TO_C_r=%d,P_TO_C_w=%d,C_TO_P_r=%d,C_TO_P_w=%d\n",i,a,b,c,d);
	}
}
int OpenFile(char* filename)
{
	int descr=open(filename,O_RDONLY);
    if(descr==-1)
	{
		printf("OPEN FILE ERROR\n");
		exit(EXIT_FAILURE);
	}    
	
	return descr;
}

//читаем в i+1 структуру из i ребенка, добавлем в W
// следующего и закрываем этого 
//если нет инфы, то она точно уже в следующем пайпе,
void r_read(int i,pChildren ChildArray,int rr,fd_set* _R,fd_set* _W,int n)
{
	int quant;
	char* buf=ChildArray[i+1].child_buf;
	int size=ChildArray[i+1].size_buf;
	quant=read(rr,buf,size);
	
	
	if(quant<0)
	{
		//printf("R_READ,CHILD=%d,quant=%d\n",i,quant);
		return;
	}
	//все инфа перед, закр rd,wr и убираем rd из R
	if(quant==0)
	{
		//printf("ffffffffffffffffffffffffffffffff,CHLD=%d\n\n\n\n\n",i);
		if(i==n-1)
			exit(1);
		int ww=(ChildArray[i+1]).pipefd_P_to_CHLD[w];
		close(ww);
		FD_CLR(rr,_R);
		close(rr);
		//printf("ERROR READ IN PARENTS\n");
		return;
	}
	//printf("R_READ,CHILD=%d,quant=%d\n",i,quant);
	ChildArray[i+1].quant=quant;
	ChildArray[i+1].first_in_buf=0;
	int ww=(ChildArray[i+1]).pipefd_P_to_CHLD[w];
	FD_CLR(rr,_R);
	FD_SET(ww,_W);

}
//записываем все из i стурктуры в pipe 
//в конце удаляем i из W, добавляем в R предыдущего
void w_write(int i,pChildren ChildArray,int ww,fd_set* _R,fd_set* _W)
{
	
	int quant=ChildArray[i].quant;
	char* buf=ChildArray[i].child_buf;
	int first_in_buf=ChildArray[i].first_in_buf;
	int j;
	//printf("                 !!!!IN W_WRITE_CHILD=%d,quant=%d,frs_buf=%d\n",i,quant,first_in_buf);
	if((j=write(ww,(buf+first_in_buf),quant-first_in_buf))<0)
	{
		printf("ERROR WRITE FROM PARENTS IN CHILD\n");
		perror("read");
		exit(1);
	}
	first_in_buf+=j;
	//printf("                 IN W_WRITE_CHILD=%d,quant=%d,frs_buf=%d\n",i,quant,first_in_buf);


	
	if(first_in_buf==quant)
	{
		ChildArray[i].first_in_buf=0;
		ChildArray[i].quant=0;
		//printf("IN W_WRITE, FRST_BUF=QUANT\n");
		int rr=(ChildArray[i-1]).pipefd_CHLD_to_P[r];
		FD_CLR(ww,_W);
		FD_SET(rr,_R);
		return;
	}
	ChildArray[i].first_in_buf=first_in_buf;
}
int main(int args,char* argv[])
{	
	if(args!=3)
	{
		printf("ERROR, MUST 2 PARAM\n");
		exit(1);
	}
	int i,pid;
	char* buf;
	int pipefd_1[2];
	int pipefd_2[2];
	int size;
	int* mas=(int*)malloc(sizeof(int));
	read_int(&mas,2,argv);
	int n=*mas;
	pChildren ChildArray=(pChildren)malloc(sizeof(Children)*(n+1));
	for(i=0;i<n;++i)
	{
		if (pipe(pipefd_1) == -1) 
		{
			printf("ERROR IN CREATE PIPE\n");
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		if (pipe(pipefd_2) == -1) 
		{
			printf("ERROR IN CREATE PIPE\n");
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		
		if(i==0)
		{
			int file_descr=OpenFile(argv[2]);
			close(pipefd_1[0]);
			close(pipefd_1[1]);
			ChildArray[i].pipefd_P_to_CHLD[r]=file_descr;
			ChildArray[i].pipefd_CHLD_to_P[0]=pipefd_2[0];
			ChildArray[i].pipefd_CHLD_to_P[1]=pipefd_2[1];
		}
		
		else
		{
			(ChildArray[i]).pipefd_P_to_CHLD[0]=pipefd_1[0];
			(ChildArray[i]).pipefd_P_to_CHLD[1]=pipefd_1[1];
			(ChildArray[i]).pipefd_CHLD_to_P[0]=pipefd_2[0];
			(ChildArray[i]).pipefd_CHLD_to_P[1]=pipefd_2[1];
		}
		size=1024;
		for (int j=0;j<(n-i);++j)
		{
			size=size*3;
			if(size>12800)
			{
				j=n-i;
				size=128000;
			}
				
		}
		
		buf=(char*)malloc(sizeof(char)*size);
		ChildArray[i].size_buf=size;
		ChildArray[i].child_buf=buf;
		ChildArray[i].first_in_buf=0;
	}
	ChildArray[n].pipefd_P_to_CHLD[w]=STDOUT_FILENO;
	ChildArray[n].size_buf=1024;
	ChildArray[n].first_in_buf=0;
	size=1024;
	buf=(char*)malloc(sizeof(char)*size);
	ChildArray[n].child_buf=buf;
	//PrintArrayChld(ChildArray,n+1);
	for(i=0;i<n;++i)
	{
		
		pid=fork(); 
		if (pid==-1)
		{
			printf("TROUBLE WITH FORK\n ");
			exit(EXIT_FAILURE);
		}
		if (pid==0)
			break;
	}
	if (pid!=0)
	{
		//РОДИТЕЛЬ
		fd_set R,W,RR,WW;
		ParentsCloseDescr(ChildArray,n);
		for(i=0;i<n;++i)
		{
			int k=fcntl(ChildArray[i].pipefd_CHLD_to_P[r],F_SETFL,O_NONBLOCK);
			if(k<0)
			{
				printf("ERROR IN FCNTL\n" );
				exit(EXIT_FAILURE);
			}
			k=fcntl(ChildArray[i].pipefd_P_to_CHLD[w],F_SETFL,O_NONBLOCK);
			if(k<0)
			{
				printf("ERROR IN FCNTL\n" );
				exit(EXIT_FAILURE);
			}
		}
		int max_fd=0;
		int rr,ww;
		FD_ZERO(&R);
		FD_ZERO(&W);
		
		for(i=0;i<n;++i)
		{
			rr=(ChildArray[i]).pipefd_CHLD_to_P[r];
			if(rr>=max_fd)
				max_fd=rr+1;
			FD_SET(rr,&R);
		}
		while(1)
		{
			RR=R;
			WW=W;
			
			//printf("                         HERE1\n");
			int l=select(max_fd,&RR,&WW,NULL,NULL);
			//printf("IN PARENTS,AFTER SELECT\n");
			//printf("                         HERE2\n");
			for(i=0;i<n;++i)
			{
				rr=(ChildArray[i]).pipefd_CHLD_to_P[r];
				ww=(ChildArray[i]).pipefd_P_to_CHLD[w];
				
				if(FD_ISSET(rr,&RR))
				{
					r_read(i,ChildArray,rr,&R,&W,n);
				}
				//printf("                         HERE3\n");
				if(FD_ISSET(ww,&WW))
				{
					w_write(i,ChildArray,ww,&R,&W);
				}
				//printf("                         HERE4\n");
			}
			
			//если дошла до конца в n+1 стуктуру
			ww=(ChildArray[n]).pipefd_P_to_CHLD[w];
			if(FD_ISSET(ww,&WW))
			{
				w_write(n,ChildArray,ww,&R,&W);
			}
			//printf("                         HERE4\n");
			//обновляем значение max_fd
			max_fd=0;
			//PrintSet(ChildArray,R,W,n);
			for(i=0;i<n;++i)
			{
				rr=(ChildArray[i]).pipefd_CHLD_to_P[r];
				ww=(ChildArray[i]).pipefd_P_to_CHLD[w];
				if(FD_ISSET(rr,&R) && rr>=max_fd)
				{
					
					max_fd=rr+1;
				}
				if(FD_ISSET(ww,&W)&& ww>=max_fd)
				{
					max_fd=ww+1;
				}
				
			}
			ww=(ChildArray[n]).pipefd_P_to_CHLD[w];
			if(FD_ISSET(ww,&W)&& ww>=max_fd)
			{
				max_fd=ww+1;
			}
		}
		
		
		
	}
	if (pid==0)
	{
		//ДЕТИ
		ChildCloseDescr(ChildArray,n,i);
		int quant,first,ex;
		char buf[1100];
		size_t size=1000;
		int r_chld;
		int w_chld;
		while(1)
		{
			r_chld=(ChildArray[i]).pipefd_P_to_CHLD[r];
			w_chld=(ChildArray[i]).pipefd_CHLD_to_P[w];
			
			sleep(1);
			//printf("BEFORE READ IN CHILD=%d\n",i);
			quant=read(r_chld,buf,size);
			//printf("AFTER READ IN CHILD=%d,quant=%d\n",i,quant);
			//значит, что все считали и при этом все считанное 
			//записали в pipe, закр дейскр и говорим что все в пайпе
			if((quant)<=0 )
			{
				//printf("ERROR READ IN CHILD=%d\n\n\n\n\n\n\n\n\n",i);
				close(ChildArray[i].pipefd_CHLD_to_P[w]);
				//perror("read");
				exit(1);
			}
			
			//printf("IN CHILD=%d AFTER READ,quant=%d\n",i,quant);
			first=0;
			//printf("IN CHLD BEFORE WHILE\n");
				//прочитали часть и передаем пока информация не закончится
			while(quant)
			{
				//printf("IN CHLD BEFORE Wr\n");
				ex=write(w_chld,&(buf[first]),quant);
				//printf("IN CHLD AFTER Wr\n");
				first+=ex;
				quant-=ex;
			}
			//printf("IN CHLD AFTER WHILE\n");
		}
	}
}
