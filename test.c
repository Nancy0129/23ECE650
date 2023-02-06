#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void * thread(void * varg) {
  int * arg = varg;
  printf("1\n");
  sleep(5);
  return NULL;
}

int main(void) {
  pthread_t thr;
  int x = 0;
  pthread_create(&thr, NULL, thread, &x);
  pthread_create(&thr, NULL, thread, &x);
  pthread_create(&thr, NULL, thread, &x);
  thread(&x);
  pthread_join(thr, NULL);
  return 0;
}
