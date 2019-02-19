#include <stdio.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
	
	printf("%s\n\n",argv[0]);
	execvp(argv[1],argv+1);
	return 0;
}
//вопрос по поводу (char**)argv[1] and argv+1
