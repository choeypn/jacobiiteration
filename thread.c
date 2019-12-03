#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "thread.h"

int main(int argc, char *argv[]){
  double in;
  int j;
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
  jacobi(M,N);

  if(f != stdin)
    fclose(f);

  return 0;
}

void jacobi(double (*M)[1024],double (*N)[1024]){
 double old = 0, new, eps;
 int done = 0; 
 eps = 0.001;
 while(!done){
  done = 1; 
  for(int i = 1; i < 1023;i++){
    for(int j = 1; j < 1023;j++){
      N[i][j] = (M[i-1][j] + M[i+1][j] + M[i][j-1] + M[i][j+1])/4.0;
    } 
  }

  for(int i = 1; i < 1023;i++){
    for(int j = 1; j < 1023;j++){
      if(fabs(M[i][j] - N[i][j]) > eps){
        done = 0;
      }
      M[i][j] = N[i][j];
    } 
  }
 }
 int l = 40;
 int k = 500;
 printf("N[%d][%d] : %.10lf \n",l,k,N[l][k]);
}
