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
      if(i == 0 || i == 1023)
        N[i][j] = in;
      else if(j == 0 || j == 1023)
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
 
  for(int i = 1; i < 1022;i++){
    for(int j = 1; j < 1022;j++){
      N[i][j] = (M[i-1][j] + M[i+1][j] + M[i][j-1] + M[i][j+1])/4.0;
    }
  }
/*
  for(int k = 0; k < 1023;k++)
    printf("N[1][%d] : %.10lf \n",k,N[1][k]);
*/
}
