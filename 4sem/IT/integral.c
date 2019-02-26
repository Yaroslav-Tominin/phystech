#define _GNU_SOURCE
#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "read_int.h"
#include <time.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
typedef struct CPU_TOPOLOGY
{
  int* buf_kern;
  int* buf_proces;
  int num_imag_process;
}CpuTop,*pCpuTop;

CpuTop ncpus()
{

  FILE *cmd = popen("cat /proc/cpuinfo | grep -i 'core id'|sort | wc -l","r");
  FILE *cmd_kern = popen("cat /proc/cpuinfo | egrep -i 'core id' | grep -o -E '[0-9]+'","r");
  FILE *cmd_proces = popen("cat /proc/cpuinfo | egrep 'processor'| grep -o -E '[0-9]+'","r");

  if (cmd == NULL || cmd_kern==NULL || cmd_proces ==NULL)
      handle_error("DONT'T OPEN FILE\n");
  printf("GFGGF\n");
  size_t n;
  int count;
  CpuTop cpu;
  if ((n = fscanf(cmd,"%d",&count)) <= 0)
      handle_error("DONT'T SCANF COUNT\n");
  int* buf_kern=(int*)malloc(sizeof(int)*count);
  int* buf_proces=(int*)malloc(sizeof(int)*count);
  cpu.buf_kern=buf_kern;
  cpu.buf_proces=buf_proces;
  cpu.num_imag_process=count;
  for(int i=0;i<count;++i)
  {
    if ((n = fscanf(cmd_kern,"%d",&(buf_kern[i]))) <= 0)
        handle_error("DONT'T READ CPU KERN\n");
    if ((n = fscanf(cmd_proces,"%d",&(buf_proces[i]))) <= 0)
        handle_error("DONT'T READ CPU PROCES\n");
    printf("Kernel:%d,procees:%d\n", buf_kern[i],buf_proces[i]);
  }

  pclose(cmd);

  return cpu;
}





typedef struct tread_info
{
  pthread_t thread_id;
  double start;
  double end;
  double step;
  double* tread_sum;
  int num_thread;
  int num_process;
}TreadInfo,*pTreadInfo;
void* calculate_integral(void* arg)
{
  clock_t cstart = clock();
  clock_t cend = 0;

  pTreadInfo tread=(pTreadInfo)arg;
  int n=(tread->end-tread->start)/tread->step;
  double k;
  cpu_set_t cpuset;
  pthread_t thread;
  CPU_ZERO(&cpuset);
  thread=pthread_self();
  CPU_SET(tread->num_process,&cpuset);
  int s=pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  if(s!=0)
    handle_error("DONT'T TAKE A CERNEL\n");
  for(int i=0;i<n;++i)
  {
    k=tread->start+tread->step*i;
    *(tread->tread_sum)+=(cos(k)+cos(k+tread->step))/2.0*(tread->step);
    //(tread->tread_sum)+=(k*k+(k+(tread->step))*(k+(tread->step)))/2*(tread->step);
    //printf("S=%f\n",sum );
  }
  cend = clock();
 printf ("THREAD=%d,%.3f cpu sec\n",tread->num_thread, ((double)cend - (double)cstart)* 1.0e-6);
  return (void*)(tread->tread_sum);
}
int main(int argc, char const *argv[])
{
  clock_t cstart = clock();
  clock_t cend = 0;
  if(argc!=2)
  {
    printf("NOT GIVE A NUMBER%d\n",argc);
    return 0;
  }
  int j;
  double a=5;
  double b=20;
  double step=0.0000001;
  double k,delta;
  int* array;
  int flag=read_int(&array,argc,argv);
  if(!flag)
  {
    printf("BED NUMBER\n" );
    return 0;
  }
  int number_treads=*array;
  free(array);
  CpuTop cpu_topology=ncpus();
  printf("NCPUS=%d\n", cpu_topology.num_imag_process);
  int logic_proces[cpu_topology.num_imag_process];
  int n=0;
  for(int i=0;i<cpu_topology.num_imag_process;++i)
  {
    if(i==0)
    {
      //printf("HERE777777,j=%d,i=%d\n",j ,i);
      logic_proces[n]=cpu_topology.buf_proces[i];
      ++n;
      continue;
    }
    j=0;
    while(j<i)
    {
      //printf("HERE11111\n" );
      if(j==i-1 && cpu_topology.buf_kern[j]!=cpu_topology.buf_kern[i])
      {
        //printf("HERE55555555,j=%d,i=%d\n",j ,i);
        logic_proces[n]=cpu_topology.buf_proces[i];
        ++n;
        ++j;
        continue;
      }
      //printf("HERE22222\n" );
      if(cpu_topology.buf_kern[j]==cpu_topology.buf_kern[i])
        break;
      //printf("HERE33333\n" );
      ++j;
    }
  }
  printf("N is value:%d,logic_pr_1=%d,log_pr_2=%d\n", n,logic_proces[0],logic_proces[1]);
  if(n<number_treads)
    number_treads=n;



double* SUM_ARRAY=(double*)malloc(sizeof(double)*number_treads*150);






  pTreadInfo tread=(pTreadInfo)malloc(sizeof(TreadInfo)*number_treads);
  for (j=0;j<number_treads;++j)
  {
    (tread[j]).start=a+(b-a)/number_treads*j;
    (tread[j]).end=a+(b-a)/number_treads*(j+1);
    (tread[j]).step=step;
    (tread[j]).tread_sum=&(SUM_ARRAY[j*200]);
    (tread[j]).num_thread=j;
    (tread[j]).num_process=logic_proces[j];
  }
  int s;
  for (j=0;j<number_treads;++j)
  {
    s=pthread_create(&((tread[j]).thread_id),NULL,&calculate_integral,&tread[j]);
    if(s!=0)
      handle_error("PTHREAD_NOT_CREATE");
  }
  void* res;
  double sum=0;
  printf ("MAIN  cpu sec %f\n", ((double)cend - (double)cstart)* 1.0e-6);
  for (j=0;j<number_treads;++j)
  {
    s=pthread_join(tread[j].thread_id,&res);
    if(s!=0)
      handle_error("PTHREAD_NOT_JOIN");
    sum+=*((double*)res);
  }









  double r_sum=sin(b)-sin(a);
  //printf("COS=%f\n", cos(4));
  printf("CALCULATE SUM=%f,\n REAL SUM=%f\n", sum,r_sum);
  return 0;
}
