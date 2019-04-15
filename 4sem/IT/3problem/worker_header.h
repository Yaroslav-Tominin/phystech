typedef struct CPU_TOPOLOGY
{
  int* buf_kern;
  int* buf_proces;
  int* buf_phys_proces;
  int num_imag_process;
}CpuTop,*pCpuTop;
CpuTop ncpus();
void* calculate_integral(void* arg);
void* calculate_integrall(void* arg);
int NLogicProcess();
double Result_Sum(double a,double b,double step,int number_treads);
