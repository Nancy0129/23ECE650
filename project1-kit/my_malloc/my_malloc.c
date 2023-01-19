
#include "my_malloc.h"
/*
// Padding the input size to the multiple of save unit
size_t padding(size_t in_size) {
  // If it is not a multiple of save unit
  if (in_size % SAVE_UNIT != 0) {
    //padding it to the smallest multiple of save unit larger than it
    return in_size - in_size % SAVE_UNIT + SAVE_UNIT;
  }
  // Else, directly return the input of size
  return in_size;
}
*/

size_t get_size(size_t in_size) {
  if (in_size % SAVE_UNIT != 0) {
    return in_size / SAVE_UNIT + 1;
  }
  return in_size / SAVE_UNIT;
}
void * cut_space(size_t * pre, size_t * curr, size_t target) {
  if ((size_t)((size_t *)(*curr) - (curr + 1)) < target + 2) {
    if (pre == free_head) {
      free_head = (size_t *)*(curr + 1);
    }
    else {
      *pre = *(curr + 1);
    }
  }
  else {
    *pre = (size_t)(curr + target + 1);
    *(curr + target + 1) = *curr;
    *(curr + target + 2) = *(curr + 1);
  }
  return curr + 1;
}

void * find_ff(size_t target) {
  size_t * curr = free_head;
  size_t curr_size;
  size_t * pre = free_head;
  while (curr != NULL) {
    curr_size = (size_t)((size_t *)(*curr) - (curr + 1));
    if (curr_size >= target) {
      return cut_space(pre, curr, target);
    }
    pre = curr + 1;
    curr = (size_t *)*(curr + 1);
  }
  return NULL;
}

void * ff_malloc(size_t size) {
  size = get_size(size);
  size_t * res = find_ff(size);
  if (res == NULL) {
    res = sbrk((size + 1) * SAVE_UNIT);
    *res = (size_t)(res + size + 1);
    return res + 1;
  }
  return res;
}

void ff_free(void * ptr) {
  size_t * to_free = (size_t *)ptr - 1;
  if (free_head == NULL) {
    *(to_free + 1) = (size_t)NULL;
    free_head = to_free;
    return;
  }
  size_t * curr = free_head;
  size_t * prev = free_head;
  while (curr != NULL && (size_t)curr < (size_t)to_free) {
    prev = curr + 1;
    curr = (size_t *)*(curr + 1);
  }
  curr = prev;
  if (*curr == (size_t)to_free) {
    *curr = *to_free;
    to_free = curr;
  }
  else {
    *(to_free + 1) = *(curr + 1);
    *(curr + 1) = (size_t)to_free;
  }
  if (*(to_free + 1) == *to_free) {
    *(to_free + 1) = *((size_t *)(*to_free) + 1);
    *to_free = *(size_t *)(*to_free);
  }
}
