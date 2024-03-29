#include <stdio.h>
#include <stdlib.h>


#include "my_malloc.h"

#ifdef FF
#define MALLOC(sz) ff_malloc(sz)
#define FREE(p) ff_free(p)
#endif
#ifdef BF
#define MALLOC(sz) bf_malloc(sz)
#define FREE(p) bf_free(p)
#endif

int main(int argc, char * argv[]) {
  const unsigned NUM_ITEMS = 10;
  int i;
  int size;
  int sum = 0;
  int expected_sum = 0;
  int * array[NUM_ITEMS];

  size = 4;
  expected_sum += size * size;
  array[0] = (int *)MALLOC(size * sizeof(int));
  printf("1\n");
  for (i = 0; i < size; i++) {
    array[0][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[0][i];
  }  //for i
  printf("2\n");
  size = 16;
  expected_sum += size * size;
  array[1] = (int *)MALLOC(size * sizeof(int));
  for (i = 0; i < size; i++) {
    array[1][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[1][i];
  }  //for i
  printf("3\n");
  size = 8;
  expected_sum += size * size;
  array[2] = (int *)MALLOC(size * sizeof(int));
  for (i = 0; i < size; i++) {
    array[2][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[2][i];
  }  //for i
  printf("4\n");
  size = 32;
  expected_sum += size * size;
  array[3] = (int *)MALLOC(size * sizeof(int));
  for (i = 0; i < size; i++) {
    array[3][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[3][i];
  }  //for i
  printf("5\n");
  FREE(array[0]);
  FREE(array[2]);
  printf("6\n");
  size = 7;
  expected_sum += size * size;
  array[4] = (int *)MALLOC(size * sizeof(int));
  for (i = 0; i < size; i++) {
    array[4][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[4][i];
  }  //for i
  printf("7\n");
  size = 256;
  expected_sum += size * size;
  array[5] = (int *)MALLOC(size * sizeof(int));
  for (i = 0; i < size; i++) {
    array[5][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[5][i];
  }  //for i
  printf("8\n");
  FREE(array[5]);
  FREE(array[1]);
  FREE(array[3]);
  printf("9\n");
  size = 23;
  expected_sum += size * size;
  array[6] = (int *)MALLOC(size * sizeof(int));
  for (i = 0; i < size; i++) {
    array[6][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[6][i];
  }  //for i
  printf("10\n");
  size = 4;
  expected_sum += size * size;
  array[7] = (int *)MALLOC(size * sizeof(int));
  for (i = 0; i < size; i++) {
    array[7][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[7][i];
  }  //for i
  printf("11\n");
  FREE(array[4]);
  printf("12\n");
  size = 10;
  expected_sum += size * size;
  array[8] = (int *)MALLOC(size * sizeof(int));
  for (i = 0; i < size; i++) {
    array[8][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[8][i];
  }  //for i
  printf("13\n");
  size = 32;
  expected_sum += size * size;
  array[9] = (int *)MALLOC(size * sizeof(int));
  printf("14_0/n");
  for (i = 0; i < size; i++) {
    array[9][i] = size;
  }  //for i
  for (i = 0; i < size; i++) {
    sum += array[9][i];
  }  //for i
  printf("14\n");
  FREE(array[6]);
  FREE(array[7]);
  FREE(array[8]);
  FREE(array[9]);
  printf("15\n");
  if (sum == expected_sum) {
    printf("Calculated expected value of %d\n", sum);
    printf("Test passed\n");
  }
  else {
    printf("Expected sum=%d but calculated %d\n", expected_sum, sum);
    printf("Test failed\n");
  }  //else

  return 0;
}
