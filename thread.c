#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include "thread.h"

#define NUMTHD 6

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
  printf("NUMBER OF THREADS : %d \n",NUMTHD); 
  clock_gettime(CLOCK_REALTIME,&TIMESPEC);
  start = TIMESPEC.tv_sec;
  
  jacobi(M,N);  
   
  clock_gettime(CLOCK_REALTIME,&TIMESPEC);
  end = TIMESPEC.tv_sec;

  printf("processing time(sec) : %d \n",end-start);

  int l = 1000;
  int k = 1000;
  printf("N[%d][%d] : %.10lf \n",l,k,N[l][k]);

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

 sem_t lock;
 sem_init(&lock,0,NUMTHD);

 for(int i = 0; i < NUMTHD;i++){ 
   thdargs[i].M = M;
   thdargs[i].N = N;
   thdargs[i].tnum = i;
   thdargs[i].idxstart = 1 + count*i;
   thdargs[i].idxend = thdargs[i].idxstart + count;
   thdargs[i].lock = &lock;
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
 sem_destroy(&lock);
}

void *thdJacobi(void *arg){
  struct threadArgs *p = arg;
  int done = 0;
  double eps = 0.00001;
  double(*m)[1024] = p->M;
  double(*n)[1024] = p->N; 
  int count;
  
  printf("thread %d starts at row %d, ends at row %d \n",p->tnum,p->idxstart,p->idxend-1);  
  while(!done){
    sem_wait(p->lock); 

    done = 1;
    for(int i = p->idxstart; i < p->idxend;i++){
      for(int j = 1; j < 1023;j++){
        n[i][j] = (m[i-1][j] + m[i+1][j] + m[i][j-1] + m[i][j+1])/4.0;
      } 
    }

    sem_getvalue(p->lock,&count);
    if(count == 0){
      for(int i = 0; i < NUMTHD;i++)
        sem_post(p->lock);
    }

    sem_wait(p->lock); 

    for(int i = p->idxstart; i < p->idxend;i++){
      for(int j = 1; j < 1023;j++){
        if(fabs(m[i][j] - n[i][j]) > eps){
          done = 0;
        }
        m[i][j] = n[i][j];
      } 
    }
    
    sem_getvalue(p->lock,&count);
    if(count == 0){
      for(int i = 0; i < NUMTHD;i++)
        sem_post(p->lock);
    }

  }
  printf("thread %d finished processing \n",p->tnum);
  return p;
}

void barrier_init(struct barrier *b,sem_t *s){
  b = malloc(sizeof(struct barrier));
  b->mtx = s;
  b->arrive = 0;
  b->leave = 0;
}

void block(struct barrier *b){
  int arrived = 0;
  sem_wait(b->mtx);
  arrived++;
  while(arrived < NUMTHD){}
  sem_post(b->mtx);
}
