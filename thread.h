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
  sem_t *lock;
};

struct barrier{
  int arrive;
  int leave;
  int flag;
  sem_t *mtx;
};

struct timespec TIMESPEC;

void jacobi(double (*M)[1024], double (*N)[1024]);

void *thdJacobi(void *arg);

void barrier_init(struct barrier *b,sem_t *s);

void block(struct barrier *b);
