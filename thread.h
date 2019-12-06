/*
  CSCI 347 Assignment#4

*/

struct threadArgs {
  pthread_t threadID;
  int tnum;
  double(*M)[1024];
  double(*N)[1024];
  int idxstart;
  int idxend;
  int *done;
};
struct timespec TIMESPEC;

void jacobi(double (*M)[1024], double (*N)[1024]);

void *thdJacobi(void *arg);
