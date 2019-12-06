#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include "thread.h"

<<<<<<< HEAD
#define NUMTHD 8
=======
#define NUMTHD 1
static pthread_barrier_t barrier;
>>>>>>> 93a5db0c86a988d596625fa8b3ab24bdfc4fc57d

int main(int argc, char *argv[]){
  double in;
  int j;
  unsigned long start,end;
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
  printf("NUMBER OF THREADS : %d \n",NUMTHD);
  puts("===================================");
  clock_gettime(CLOCK_MONOTONIC,&TIMESPEC);
  start = TIMESPEC.tv_sec;
<<<<<<< HEAD
  
  jacobi(M,N);  
   
  clock_gettime(CLOCK_MONOTONIC,&TIMESPEC);
=======

  jacobi(M,N);

  clock_gettime(CLOCK_REALTIME,&TIMESPEC);
>>>>>>> 93a5db0c86a988d596625fa8b3ab24bdfc4fc57d
  end = TIMESPEC.tv_sec;

  puts("===================================");
  printf("processing time(sec) : %ld \n",end-start);

  puts("===================================");
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

  struct threadArgs thdargs[NUMTHD];
  int count;
  count = 1022/NUMTHD;
  int barCode;
  int done = 0;

  //Create Barrier and Check for Success
  barCode = pthread_barrier_init(&barrier, NULL, NUMTHD);
  if (barCode != 0)
  perror("pthread_barrier_init");


  for(int i = 0; i < NUMTHD;i++){
    thdargs[i].M = M;
    thdargs[i].N = N;
    thdargs[i].tnum = i;
    thdargs[i].idxstart = 1 + count*i;
    thdargs[i].idxend = thdargs[i].idxstart + count;
    thdargs[i].done = &done;
    if(i == NUMTHD-1)
    thdargs[i].idxend = 1023;
    if(pthread_create(&thdargs[i].threadID,NULL,thdJacobi,&thdargs[i]))
    perror("thread error");
  }

  for(int i =0; i < NUMTHD;i++){
    void *p;
    if(pthread_join(thdargs[i].threadID,&p))
    perror("join error");
  }
  int pthread_barrier_destroy(pthread_barrier_t *barrier);
}

void *thdJacobi(void *arg){
  struct threadArgs *p = arg;
  double eps = 0.00001;
  double(*m)[1024] = p->M;
  double(*n)[1024] = p->N;
  int count = NUMTHD;
  int barCode;

  printf("thread %d starts at row %d, ends at row %d \n",p->tnum,p->idxstart,p->idxend);

  while(!*p->done){

    barCode = pthread_barrier_wait(&barrier);

    *p->done = 1;

    for(int i = p->idxstart; i < p->idxend;i++){
      for(int j = 1; j < 1023;j++){
        n[i][j] = (m[i-1][j] + m[i+1][j] + m[i][j-1] + m[i][j+1])/4.0;
      }
    }

    barCode = pthread_barrier_wait(&barrier);

    for(int i = p->idxstart; i < p->idxend;i++){
      for(int j = 1; j < 1023;j++){
        if(fabs(m[i][j] - n[i][j]) > eps){
          *p->done = 0;
        }
        m[i][j] = n[i][j];
      }
    }

      barCode = pthread_barrier_wait(&barrier);

}
  printf("thread %d finished processing \n",p->tnum);
  return p;
}
