/*
  CSCI 347 Assignment#4

*/

struct threadArgs {
  int tnum;
  double(*M)[1024];
  double(*N)[1024];
};

struct timespec TIMESPEC;

void jacobi(double (*M)[1024], double (*N)[1024]);

void *thdJacobi(void *arg);
