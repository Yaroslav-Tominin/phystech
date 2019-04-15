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
#include <netinet/tcp.h>
#define PORT "4000"
#define port 4000


#define handle_error(msg) \
               do { perror(msg); exit(1); } while (0)
typedef struct CalculateIntegral
{
 double a;
 double b;
 double step;
}CalculateIntegral,*pCalculateIntegral;
int main(int argc, char const *argv[]) {
  double A=35;
  int optval;
  socklen_t optlen;
  double B=50000;
  double step=0.001;
  int udp_sockfd,k,clos;
  //INADDR_BROADCAST позволяет рассылать всем ip из локальной сети
  struct sockaddr_in udp_workers_addr=
  {
    .sin_family=AF_INET,
    .sin_addr.s_addr=htonl(INADDR_BROADCAST),
    .sin_port=htons(port),
  };
  memset(udp_workers_addr.sin_zero, '\0', sizeof udp_workers_addr.sin_zero);
  /*стартер посылает всем воркерам сообщение и
   ждет пока они откликнутся
  */
  printf("UDP_SOCK\n");
  if((udp_sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1)
    handle_error("SOCKFD\n");
  int val=1;
  //bind вроде как тоже не нужен
  //if((ret=bind(sockfd,&starter_addr,sizeof(starter_addr)))<0)
  //  handle_error("BIND PROBL\n");
  //делаем его широковещательным
  printf("UDP SETSOCOPT\n");
  if((setsockopt(udp_sockfd,SOL_SOCKET,SO_BROADCAST,&val,sizeof(val)))<0)
    handle_error("SETSOCKOPT PROBL\n");


  //теперь должны принимать сообщение от воркеров, то есть lisen уже должен слушать перед broadcast'ингом
  /*
  sockaddr_in
  socket
  listen
  accept
  */
  int tcp_sockfd;
  struct addrinfo tcp_starter,*tcp_res,*p;
  memset(&tcp_starter,0,sizeof(tcp_starter));
  tcp_starter.ai_family=AF_UNSPEC;
  tcp_starter.ai_socktype=SOCK_STREAM;
  tcp_starter.ai_flags=AI_PASSIVE;
  k=getaddrinfo(NULL,PORT,&tcp_starter,&tcp_res);
  if(k)
    handle_error("GETADDRINFO_to problem\n");
  printf("TCP_SOCK\n");
  if((tcp_sockfd=socket(tcp_res->ai_family,tcp_res->ai_socktype,tcp_res->ai_protocol))==-1)
    handle_error("TCP SOCKET problem\n");
  //если с прошлого раза дескриптор находится в режиме TIME-WAIT, то все равно юзаем
  int enable=1;
  if (setsockopt(tcp_sockfd, SOL_SOCKET, SO_REUSEADDR,&enable,sizeof(int)) < 0)
    handle_error("STARTER TCP SETSOCKOPT WORKER");
  printf("TCP BIND\n");
  k=bind(tcp_sockfd,tcp_res->ai_addr,tcp_res->ai_addrlen);
  if(k==-1)
    handle_error("TCP BIND problem\n");
  //начинаем слушать worker'ов
  printf("TCP LISTEN\n");
  k=listen(tcp_sockfd,10);



  //рассылаем всем воркерам
  printf("UDP SENDTO\n");
  k=sendto(udp_sockfd,"1",1,0,(struct sockaddr*)&udp_workers_addr,sizeof(udp_workers_addr));
  if(k==-1)
    handle_error("SEND_to problem\n");
  close(udp_sockfd);
  struct sockaddr tcp_addr[20];
  int tcp_worker_sock[20];
  int count_worker=0;
  struct timeval tv =
  {
    .tv_sec = 0,
    .tv_usec = 50000
  };

  //сейчас принимаем accept'ом ответ от воркеров и даем им задачи через структуру
  int tcp_size_accept=sizeof(struct sockaddr);
  if (setsockopt(tcp_sockfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0)
    handle_error("STARTER TCP SETSOCKOPT WORKER");
  printf("TCP ACCEPT\n");
  while((tcp_worker_sock[count_worker]=accept(tcp_sockfd,&(tcp_addr[count_worker]),&tcp_size_accept))!=-1)
  {
    printf("THERE IS WORKER\n");
    //меняем блокируюший режим сокета, чтобы не ждать воркера долго
    if (setsockopt(tcp_worker_sock[count_worker], SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0)
      handle_error("STARTER TCP SETSOCKOPT WORKER");
    ++count_worker;
    if(count_worker==20)
      handle_error("The number worker more 20, need the more size\n");

  }
  //we should recv a number logic proces from worker
  int size,s,final;
  int divide_into_parts=0;
  int workers_number_logic_process[count_worker];
  //contain on i place the sum of 0,...,i-1 workers_number_logic_process
  int for_divide_task[count_worker+1];
  for(k=0;k<count_worker;++k)
  {
    final=sizeof(int);
    size=0;
    while(size!=sizeof(int))
    {
      s=recv(tcp_worker_sock[k],((void*)&(workers_number_logic_process[k]))+size,final,0);
      if(s<=0)
        handle_error("PROBLEM RECV A MESSAGE\n");
      final-=s;
      size+=s;
    }
    for_divide_task[k]=divide_into_parts;
    divide_into_parts+=workers_number_logic_process[k];
  }
  for_divide_task[count_worker]=divide_into_parts;
  printf("THE NUMBER OF DIVIDE PARTS %d\n",divide_into_parts );
  //we have count workers and let's give him the problems
  CalculateIntegral task[count_worker];
  for(k=0;k<count_worker;++k)
  {
    task[k].a=A+(B-A)/divide_into_parts*for_divide_task[k];
    task[k].b=A+(B-A)/divide_into_parts*for_divide_task[k+1];
    task[k].step=step;
    //посылаем, пока не передадим всю структуру
    printf("TCP_SEND\n");
    printf("WORKER:%d,a:%f,b:%f\n",k,task[k].a,task[k].b );
    //sleep(5);
    size=0;
    final=sizeof(CalculateIntegral);
    while(size!=sizeof(CalculateIntegral))
    {
      s=send(tcp_worker_sock[k],((void*)&(task[k]))+size,final,0);
      if(s<=0)
        handle_error("PROBLEM SEND A MESSAGE\n");
      final-=s;
      size+=s;
    }
  }
    //нагрузили всех воркеров, теперь ждем ответа
    printf("NUMBER WORKER:%d\n",count_worker);
    double worker_sum[count_worker];
    tv.tv_usec=0;
    for(k=0;k<count_worker;++k)
    {
      final=sizeof(double);
      size=0;
      s=0;
      //cделаем keepalive, чтобы не зависнуть при экстренном выключении
      optval = 1;
      optlen = sizeof(optval);
      if(setsockopt(tcp_worker_sock[k], SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0)
        handle_error("STARTER TCP SETSOCKOPT KEEPALIVE\n");
      if(setsockopt(tcp_worker_sock[k], SOL_TCP, TCP_KEEPIDLE, &optval, optlen) < 0)
        handle_error("STARTER TCP SETSOCKOPT KEEPALIVE\n");
      if(setsockopt(tcp_worker_sock[k], SOL_TCP, TCP_KEEPINTVL, &optval, optlen) < 0)
        handle_error("STARTER TCP SETSOCKOPT KEEPALIVE\n");
      if(setsockopt(tcp_worker_sock[k], SOL_TCP, TCP_KEEPCNT, &optval, optlen) < 0)
        handle_error("STARTER TCP SETSOCKOPT KEEPALIVE\n");

      //не хотим зависнуть надолго, хочется поставить таймаут
      printf("TCP_RECV\n");
      //меняем таймаут на блокируюший режим
      if (setsockopt(tcp_worker_sock[k], SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0)
        handle_error("STARTER TCP SETSOCKOPT WORKER");
      while(size!=sizeof(double))
      {
        s=recv(tcp_worker_sock[k],((void*)&(worker_sum[k]))+size,final,0);
        if(s<=0)
          handle_error("PROBLEM SEND A MESSAGE\n");
        final-=s;
        size+=s;
      }
      clos=close(tcp_worker_sock[k]);
      if(clos==-1)
        handle_error("TCP_WORKER_SOCK PROBLEM CLOSE\n");
    }
    //calculate the worker_sum
    double SUM;
    for(k=0;k<count_worker;++k)
      SUM+=worker_sum[k];
    printf("The final sum in %f\n",SUM);
    clos=close(tcp_sockfd);
    if(clos==-1)
      handle_error("TCP_SOCKFD PROBLEM CLOSE\n");

  return 0;
}
