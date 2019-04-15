#include "worker_header.h"
#include "read_int.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>
#include <math.h>
#include <sys/termios.h>
#define PORT "4010"
#define port 4010
#define handle_error(msg) \
               do { perror(msg); exit(1); } while (0)
typedef struct CalculateIntegral
{
  double a;
  double b;
  double step;
}CalculateIntegral,*pCalculateIntegral;
int main(int argc, char const *argv[]) {
  /*
  sockaddr_in worker
  socket
  BIND
  sendto#include "worker_header.h"
  */
  //изначально получим количество логичесских процессоров на нашей машине, чтобы послать их когда потребуется
  if(argc!=2)
  {
    printf("NOT GIVE A NUMBER%d\n",argc);
    return 0;
  }
  int* array;
  int flag=read_int(&array,argc,argv);
  if(!flag)
  {
    printf("BED NUMBER\n" );
    return 0;
  }
  int number_treads=*array;
  free(array);
  int number_of_logic_process=NLogicProcess();
  if(number_of_logic_process>number_treads)
    number_of_logic_process=number_treads;
  int k,clos,size,final;
  int udp_sockfd;
  char buf[129];
  struct sockaddr_in udp_starter_addr;
  struct addrinfo udp_worker_addr,*udp_res;
  memset(&udp_worker_addr, 0, sizeof udp_worker_addr);
  udp_worker_addr.ai_family = AF_UNSPEC;
  udp_worker_addr.ai_socktype = SOCK_DGRAM;
  udp_worker_addr.ai_flags = AI_PASSIVE;
  // использовать мой IP
  //создаем сокет, чтобы получить сообщение от starter
  printf("BEFORE CREATE SOCK UDP FOR RECVFROM MESSAGE FRO STARTER\n" );
  if ((k = getaddrinfo(NULL, PORT, &udp_worker_addr, &udp_res)) != 0)
    handle_error("WORKER UDP GETADDRINFO\n");
  if((udp_sockfd=socket(udp_res->ai_family,udp_res->ai_socktype,udp_res->ai_protocol))==-1)
    handle_error("WORKER UDP SOCKET problem\n");
  printf("BEFORE BIND SOCK UDP FOR RECVFROM MESSAGE FRO STARTER\n" );
  k=bind(udp_sockfd,udp_res->ai_addr,udp_res->ai_addrlen);
  if(k==-1)
    handle_error("WORKER UDP BIND problem\n");
  int udp_starter_struc_size=sizeof(struct sockaddr_in);
  printf("BEFORE RECVFROM MESSAGE FRO STARTER\n" );
  k=recvfrom(udp_sockfd,&buf,128,0,(struct sockaddr*)&udp_starter_addr,&udp_starter_struc_size);
  if(k==-1)
    handle_error("WORKER UDP RECVFROM problem\n");
  clos=close(udp_sockfd);
  if(clos==-1)
    handle_error("UDP_SOCKFD PROBLEM CLOSE\n");
  //теперь должны создать tcp socket и отправить запрос на coonect


  int tcp_socket;
  printf("BEFORE CREATE SOCK TCP FOR RECV PROBLEM\n" );
  if((tcp_socket=socket(PF_INET,SOCK_STREAM,0))==-1)
    handle_error("WORKER TCP SOCKET problem\n");
  //найдем ip, которое лежит в структуре udp_starter_addr
  //в структуре лежит произвольныейй порт: так как: когда старктер делает sendto,
  //то ищется произвольный порт, а нам нужен 4000

  printf("BEFORE CONNECT SOCK TCP FOR RECV PROBLEM\n" );
  udp_starter_addr.sin_port=htons(port);
  k=connect(tcp_socket,(struct sockaddr*)&udp_starter_addr,sizeof(struct sockaddr));
  if(k==-1)
    handle_error("WORKER TCP CONNECT problem\n");
  //после подключения хотим передать сколько у нас логич процессоров, чтобы стартер поделил задачу
  int s;
  size=0;
  final=sizeof(int);
  while(size!=sizeof(int))
  {
    s=send(tcp_socket,((void*)&(number_of_logic_process))+size,final,0);
    if(s<=0)
      handle_error("PROBLEM SEND A NUMBER LOGIC PROCESS MESSAGE\n");
    final-=s;
    size+=s;
  }


  //должны получить структуру, заранее оговоренного размера
  CalculateIntegral task;
  final=sizeof(CalculateIntegral);
  size=0;

  printf("BEFORE RECV PROBLEM\n" );
  //sleep(5);
  while(size!=sizeof(CalculateIntegral))
  {
    printf("HERE\n");
    s=recv(tcp_socket,((void*)&(task))+size,final,0);
    if(s<=0)
      handle_error("PROBLEM RECV A MESSAGE\n");
    final-=s;
    size+=s;
  }
  printf("TASK,a:%f,b:%f\n",task.a,task.b );
  //теперь решаем полученную задачу
  double sum=Result_Sum(task.a,task.b,task.step,number_of_logic_process);

  size=0;
  final=sizeof(double);
  printf("BEFORE SPEND SOLUTION,workers sum=%f\n",sum );
  while(size!=sizeof(double))
  {
    s=send(tcp_socket,((void*)&sum)+size,final,0);
    if(s==-1)
      handle_error("PROBLEM SEND A MESSAGE\n");
    final-=s;
    size+=s;
  }
  //все передали, поэтому отключаемся
  clos=close(tcp_socket);
  if(clos==-1)
    handle_error("TCP_SOCKET PROBLEM CLOSE\n");
  return 0;
}
