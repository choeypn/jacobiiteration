#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
      //printf("N[%d][%d] = %.10lf \n",i,j,N[i][j]);
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
 int k = 0, state; 
 eps = 0.002;
 while(k < 1000){
  state = 1; 
  for(int i = 1; i < 1023;i++){
    for(int j = 1; j < 1023;j++){ 
      new = (N[i-1][j] + N[i+1][j] + N[i][j-1] + N[i][j+1])/4.0;
      if(!new-N[i][j] < eps){
        N[i][j] = new;
        state = 0;
      }
    }
  }
 if(state)
   break;
 k++;
 }
  //for(int k = 0; k < 1023;k++)
    printf("N[101][102] : %.10lf \n",N[101][102]);
}
