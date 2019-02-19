#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int pipefd[2];
    int n=150;
    pid_t cpid;
    ssize_t t;
    char* buf=(char*)malloc(sizeof(char)*200);
    
    int fil;
    if (argc != 2) 
    {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(EXIT_FAILURE);
    }
    if (pipe(pipefd) == -1) 
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {    /* Child reads from pipe */
        close(pipefd[0]);          /* Close unused read end */
        //хотим открыть файл
        FILE* f;
        if(!(f=fopen(argv[1],"rt")))
        {
			fprintf(stderr, "Usage: %s <string>\n", argv[0]);
			exit(EXIT_FAILURE);
		}
		
        
        fil=fileno(f);
        t=read(fil, buf, n);
        while(t>0)
        {
			write(pipefd[1], buf, t);
			t=read(fil, buf, n);
		}
		fclose(f);
		
        
        
        //write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]);          /* Reader will see EOF */
        exit(EXIT_SUCCESS);
    } else {            /* Parent writes argv[1] to pipe */
        close(pipefd[1]);          /* Close unused write end */
        t=read(pipefd[0], buf, n);
        while (t > 0)
        {
            write(STDOUT_FILENO, buf, t);
            t=read(pipefd[0], buf, n);
        }
        //write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        _exit(EXIT_SUCCESS);
        wait(NULL);                /* Wait for child */
    }
}
