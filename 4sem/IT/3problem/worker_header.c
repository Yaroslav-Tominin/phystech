#define _GNU_SOURCE
#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)
#include "worker_header.h"
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
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
CpuTop ncpus()
{
  FILE *cmd_phys_process = popen("cat /proc/cpuinfo | egrep -i 'physical id' | grep -o -E '[0-9]+'","r");
  FILE *cmd = popen("cat /proc/cpuinfo | grep -i 'core id'|sort | wc -l","r");
  FILE *cmd_kern = popen("cat /proc/cpuinfo | egrep -i 'core id' | grep -o -E '[0-9]+'","r");
  FILE *cmd_proces = popen("cat /proc/cpuinfo | egrep 'processor'| grep -o -E '[0-9]+'","r");

  if (cmd == NULL || cmd_kern==NULL || cmd_proces ==NULL)
      handle_error("DONT'T OPEN FILE\n");
  //printf("GFGGF\n");
  size_t n;
  int count;
  CpuTop cpu;
  if ((n = fscanf(cmd,"%d",&count)) <= 0)
      handle_error("DONT'T SCANF COUNT\n");

  int* buf_phys_proces=(int*)malloc(sizeof(int)*count);
  int* buf_kern=(int*)malloc(sizeof(int)*count);
  int* buf_proces=(int*)malloc(sizeof(int)*count);
  cpu.buf_kern=buf_kern;
  cpu.buf_proces=buf_proces;
  cpu.buf_phys_proces=buf_phys_proces;
  cpu.num_imag_process=count;
  int i;
  for(i=0;i<count;++i)
  {
    if ((n = fscanf(cmd_kern,"%d",&(buf_kern[i]))) <= 0)
        handle_error("DONT'T READ CPU KERN\n");
    if ((n = fscanf(cmd_proces,"%d",&(buf_proces[i]))) <= 0)
        handle_error("DONT'T READ CPU PROCES\n");
    if ((n = fscanf(cmd_phys_process,"%d",&(buf_phys_proces[i]))) <= 0)
        handle_error("DONT'T READ CPU PHYS PROCES\n");

    printf("Kernel:%d,procees:%d,PHYS PROCES:%d\n", buf_kern[i],buf_proces[i],buf_phys_proces[i]);
  }

  pclose(cmd);

  return cpu;
}
void* calculate_integral(void* arg)
{
  //printf("%llu\n", current_timestamp());
  pTreadInfo tread=(pTreadInfo)arg;
  int n=(tread->end-tread->start)/tread->step;
  double k;
  cpu_set_t cpuset;
  pthread_t thread;
  CPU_ZERO(&cpuset);
  thread=pthread_self();
  CPU_SET(tread->num_process,&cpuset);
  //printf("%llu\n", current_timestamp());
  int s=pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  if(s!=0)
    handle_error("DONT'T TAKE A CERNEL\n");

  int i;
  printf("IN CALCULATE INTEGRAL,START=%f,END=%f,step=%f,SUM=%f\n",tread->start,tread->end,tread->step,*(tread->tread_sum));
  for(i=0;i<n;++i)
  {
    k=tread->start+tread->step*i;
    //*(tread->tread_sum)+=(cos(k)+cos((k+tread->step)))/2.0*(tread->step);
    *(tread->tread_sum)+=((k*k)+(k+tread->step)*(k+tread->step))/2.0*(tread->step);

  }
  printf("thread:%d,num_process:%d\n",tread->num_thread,tread->num_process );
  printf("IN CALCULATE INTEGRAL,SUM=%f\n",*(tread->tread_sum));
  return (void*)(tread->tread_sum);
}
void* calculate_integrall(void* arg)
{
  //printf("%llu\n", current_timestamp());
  pTreadInfo tread=(pTreadInfo)arg;
  int n=(tread->end-tread->start)/tread->step;
  double k;
  cpu_set_t cpuset;
  pthread_t thread;
  CPU_ZERO(&cpuset);
  thread=pthread_self();
  CPU_SET(tread->num_process,&cpuset);
  //printf("%llu\n", current_timestamp());
  int s=pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  if(s!=0)
    handle_error("DONT'T TAKE A CERNEL\n");

  int i;
  for(i=0;i<n;++i)
  {
    k=tread->start+tread->step*i;
    //*(tread->tread_sum)+=(cos(k)+cos((k+tread->step)))/2.0*(tread->step);
    *(tread->tread_sum)+=((k*k)+(k+tread->step)*(k+tread->step))/2.0*(tread->step);
  }
  printf("LOSE:thread:%d,num_process:%d\n",tread->num_thread,tread->num_process );
  return (void*)(tread->tread_sum);
}
int NLogicProcess()
{
  int j;
  double k,delta;
  CpuTop cpu_topology=ncpus();
  int n=0;
  int i;
  for(i=0;i<cpu_topology.num_imag_process;++i)
  {
    if(i==0)
    {
      ++n;
      continue;
    }
    j=0;
    while(j<i)
    {
      if((j==i-1) && (cpu_topology.buf_kern[j]!=cpu_topology.buf_kern[i]))
      {
        ++n;
        ++j;
        continue;
      }
      if(j==i-1 && cpu_topology.buf_kern[j]==cpu_topology.buf_kern[i] && cpu_topology.buf_phys_proces[j]!=cpu_topology.buf_phys_proces[i])
      {
        ++n;
        ++j;
        continue;
      }
      if(cpu_topology.buf_kern[j]==cpu_topology.buf_kern[i] && cpu_topology.buf_phys_proces[j]==cpu_topology.buf_phys_proces[i])
        break;
      ++j;
    }
  }
  return n;
}
double Result_Sum(double a,double b,double step,int number_treads)
{
  int j;
  double k,delta;
  CpuTop cpu_topology=ncpus();

  //printf("NCPUS=%d\n", cpu_topology.num_imag_process);
  int logic_proces[cpu_topology.num_imag_process];
  int n=0;
  int i;
  for(i=0;i<cpu_topology.num_imag_process;++i)
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
      //printf("HEREJHBCBQBCHQR,j=%d,i=%d,n=%d\n",j ,i,n);
      if((j==i-1) && (cpu_topology.buf_kern[j]!=cpu_topology.buf_kern[i]))
      {
        //printf("HERE55555555,j=%d,i=%d,n=%d\n",j ,i,n);
        logic_proces[n]=cpu_topology.buf_proces[i];
        ++n;
        ++j;
        continue;
      }
      if(j==i-1 && cpu_topology.buf_kern[j]==cpu_topology.buf_kern[i] && cpu_topology.buf_phys_proces[j]!=cpu_topology.buf_phys_proces[i])
      {
        //printf("HEREJJJJJJ,j=%d,i=%d,n=%d\n",j ,i,n);
        logic_proces[n]=cpu_topology.buf_proces[i];
        ++n;
        ++j;
        continue;
      }
      //printf("HERE22222\n" );
      if(cpu_topology.buf_kern[j]==cpu_topology.buf_kern[i] && cpu_topology.buf_phys_proces[j]==cpu_topology.buf_phys_proces[i])
        break;
      //printf("HERE33333\n" );
      ++j;
    }
  }
  //printf("N is value:%d,logic_pr_1=%d,log_pr_2=%d\n,%d,%d,%d,%d,", n,logic_proces[0],logic_proces[1],logic_proces[2],logic_proces[3],logic_proces[4],logic_proces[5]);
  if(n<number_treads)
    number_treads=n;
double* SUM_ARRAY=(double*)malloc(sizeof(double)*n*250);






  pTreadInfo tread=(pTreadInfo)malloc(sizeof(TreadInfo)*n);
  for (j=0;j<n;++j)
  {
    (tread[j]).start=a+(b-a)/number_treads*j;
    (tread[j]).end=a+(b-a)/number_treads*(j+1);
    (tread[j]).step=step;
    SUM_ARRAY[j*250]=0;
    (tread[j]).tread_sum=&(SUM_ARRAY[j*250]);
    (tread[j]).num_thread=j;
    (tread[j]).num_process=logic_proces[n-j-1];
  }
  int s;
  for (j=0;j<n;++j)
  {
    printf("MAIN:CREATE NORMAL THRED\n");
    s=pthread_create(&((tread[j]).thread_id),NULL,&calculate_integral,&tread[j]);
    if(s!=0)
      handle_error("PTHREAD_NOT_CREATE");
  }
  void* res;
  double sum=0;
  //cend = clock();
  //printf ("MAIN  cpu sec %f\n", ((double)cend - (double)cstart)* 1.0e-6);

  for (j=0;j<number_treads;++j)
  {
    s=pthread_join(tread[j].thread_id,&res);
    if(s!=0)
      handle_error("PTHREAD_NOT_JOIN");
    if(j<number_treads)
      sum+=*((double*)res);
    printf("SUM %f\n", sum);
  }




  double r_sum=sin(b)-sin(a);
  //printf("COS=%f\n", cos(4));
  printf("CALCULATE SUM=%f,\n REAL SUM=%f\n", sum,r_sum);
  return sum;
}
