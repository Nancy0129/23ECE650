#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
typedef size_t * pointer;
void * tryInit() {
  static pointer * free_head = NULL;

  void * new_regoin = sbrk(8 * 2 + 8);
  if (free_head == NULL) {
    free_head = new_regoin;
  }
  *free_head = new_regoin;
  printf("new region: %p\n free head: %p\n", new_regoin, *free_head);
  free_head = free_head + 3;
}
int main(void) {
  tryInit();
  tryInit();
  tryInit();
  tryInit();
  printf("NULL: %lu\n", (size_t)NULL);
  return 0;
}
