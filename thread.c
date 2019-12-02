#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
      //printf("M[%d][%d] = %.10lf \n",i,j,M[i][j]);
      fscanf(f,"%lf",&in);
      ++j;
    }
  }
  if(f != stdin)
    fclose(f);

  return 0;
}
