#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void CreateFifoTogether(char* fname);
char* CreateFifoSeparate(pid_t pid);
int OpenFile(char* filename);

//Who is opening file and writing in SEPARATE_FIFO
void Reader(int argc, char *argv[]);
/*It create a separate fifo and write pid in FIFO_TOGETHER,
 after this it listen the separate fifo
*/
void Writer();
