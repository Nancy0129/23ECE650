#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SAVE_UNIT sizeof(size_t)
size_t * free_head = NULL;
size_t total_size = 0;

size_t get_size(size_t in_size) {
  if (in_size % SAVE_UNIT != 0) {
    return in_size / SAVE_UNIT + 1;
  }
  return in_size / SAVE_UNIT;
}
size_t * cut_space(size_t * pre, size_t * curr, size_t target,size_t curr_size) {
  if (curr_size-target <2) {
    // printf("%lu  %lu",)
    if (pre == free_head) {
      free_head = (size_t *)*(curr + 1);
    }
    else {
      *(pre+1) = *(curr + 1);
    }
  }
  else {
    *(curr + target + 1) = *curr;
    *(curr + target + 2) = *(curr + 1);
    *curr=(size_t)(curr + target + 1);
    if (pre == free_head) {
      free_head = curr + target + 1;
    }
    else {
     
      *(pre+1) = (size_t)(curr + target + 1);
    }    
  }
  return curr + 1;
}

size_t * find_ff(size_t target) {
  size_t * curr = free_head;
  size_t curr_size;
  size_t * pre = free_head;
  while (curr != NULL) {
    curr_size = (size_t)((size_t *)(*curr) - (curr + 1));
    if (curr_size >= target) {
      return cut_space(pre, curr, target,curr_size);
    }
    
    pre = curr ;
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
    total_size += (size + 1) * SAVE_UNIT;
    return res + 1;
  }
  return res;
}

void ff_free(void * ptr) {
  // printf("2");
  size_t * to_free = (size_t *)ptr - 1;
  if (free_head == NULL) {
    *(to_free + 1) = (size_t)NULL;
    free_head = to_free;
    return;
  }
  size_t * curr = free_head;
  size_t * prev = free_head;
  while (curr != NULL && (size_t)curr < (size_t)to_free) {
    prev = curr;
    curr = (size_t *)*(curr + 1);
  }
  if (*prev == (size_t)to_free) {
    *prev = *to_free;
    to_free = prev;
  }
  else {
    if (prev == free_head) {
      *(to_free + 1) = (size_t)free_head;
      free_head = to_free;
    }

    else {
      *(to_free + 1) = (size_t)curr;
      *(prev + 1) = (size_t)to_free;
    }
  }

  if (*to_free == (size_t)curr) {
    *(to_free + 1) = *(curr + 1);
    *to_free = *curr;
  }
}

unsigned long get_data_segment_size() {
  return total_size;
}

unsigned long get_data_segment_free_space_size() {
  // printf("3");
  unsigned long res = 0;
  size_t * curr = free_head;
  while (curr != NULL) {
    res += *curr - (unsigned long)curr;
    curr = (size_t *)*(curr + 1);
    //printf("%lu,   %lu\n", res, (size_t)curr);
  }
  return res;
}
