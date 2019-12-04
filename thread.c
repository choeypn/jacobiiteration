#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include "thread.h"

int main(int argc, char *argv[]){
  double in;
  int j,start,end;
  FILE *f;
  double(*M)[1024] = malloc(sizeof(double)*1024*1024);
  double(*N)[1024] = malloc(sizeof(double)*1024*1024);
  f = fopen("input.mtx","r");
  if(f == NULL)
    exit(1);

  fscanf(f,"%lf",&in);
  for(int i =0; i < 1024;i++){
    j = 0;
    while(j < 1024){
      M[i][j] = in;
      N[i][j] = in;
      fscanf(f,"%lf",&in);
      j++;
    }
  }
  
  clock_gettime(CLOCK_REALTIME,&TIMESPEC);
  start = TIMESPEC.tv_sec;
  
  jacobi(M,N);  
   
  clock_gettime(CLOCK_REALTIME,&TIMESPEC);
  end = TIMESPEC.tv_sec;
  printf("%d - %d = %d \n",start,end,end-start);
  if(f != stdin)
    fclose(f);
  return 0;
}

/* 
  Jacobi iteration function 
  Read matrix M and assign values to N.
  Keep iterating until every element is in stable state.
*/
void jacobi(double (*M)[1024],double (*N)[1024]){

 pthread_t mythread;
 struct threadArgs thdargs;
 thdargs.M = M;
 thdargs.N = N;
 if(pthread_create(&mythread,NULL,thdJacobi,&thdargs))
   perror("thread error");

 void *p; 
 if(pthread_join(mythread,&p))
   perror("join error"); 

}

void *thdJacobi(void *arg){
  struct threadArgs *p = arg;
  int done = 0;
  double eps = 0.001;
  double(*m)[1024] = p->M;
  double(*n)[1024] = p->N; 

  while(!done){
    done = 1; 
    for(int i = 1; i < 1023;i++){
      for(int j = 1; j < 1023;j++){
        n[i][j] = (m[i-1][j] + m[i+1][j] + m[i][j-1] + m[i][j+1])/4.0;
      } 
    }

    for(int i = 1; i < 1023;i++){
      for(int j = 1; j < 1023;j++){
        if(fabs(m[i][j] - n[i][j]) > eps){
          done = 0;
        }
        m[i][j] = n[i][j];
      } 
    }
  }
  puts("thread finished processing");
  return p;
}
