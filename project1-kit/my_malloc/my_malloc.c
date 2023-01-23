
#include "my_malloc.h"

size_t get_size(size_t in_size) {
  if (in_size % SAVE_UNIT != 0) {
    return in_size / SAVE_UNIT + 1;
  }
  return in_size / SAVE_UNIT;
}
size_t calculate_space(size_t * node){
  size_t* begin=node + 1;
  size_t* end=(size_t*)(*node);
  return (size_t)(end-begin);
}

void* setup_new(size_t size){
  size_t * res = sbrk((size + 1) * SAVE_UNIT);
    if((void*)res==(void*)-1){
      return NULL;
    }
    *res = (size_t)(res + size + 1);
    total_size += (size + 1) * SAVE_UNIT;
    return (void*)(res + 1);
}

size_t * cut_space(size_t * pre, size_t * curr, size_t target,size_t curr_size) {
  if (curr_size-target < 2) {
    if (curr == free_head) {
      free_head = (size_t *)(*(curr + 1));
    }
    else {
      *(pre+1) = *(curr + 1);
    }
  }
  else {
    *(curr + target + 1) = *curr;
    *(curr + target + 2) = *(curr + 1);
    *curr=(size_t)(curr + target + 1);
    if (curr == free_head) {
      free_head = curr + target + 1;
    }
    else {     
      *(pre+1) = (size_t)(curr + target + 1);
    }    
  }
  return curr + 1;
}

void my_free(void * ptr) {
  if(ptr==NULL){
    return;
  }
  size_t * to_free = (size_t *)ptr - 1;
  if (free_head == NULL) {
    *(to_free + 1) = (size_t)NULL;
    free_head = to_free;
    return;
  }
  size_t * curr = free_head;
  size_t * prev=NULL;
  while (curr != NULL && (size_t)curr < (size_t)to_free) {
    prev = curr;
    curr = (size_t *)*(curr + 1);
  }
  if(curr==free_head){
    *(to_free+1)=(size_t)curr;
    free_head=to_free;
  }
  else{
    *(to_free+1)=(size_t)curr;
    *(prev+1)=(size_t)(to_free);
  }
  if(prev!=NULL){
    if(*prev==(size_t)to_free){
      *(prev+1)=*(to_free+1);
      *prev=*to_free;
      to_free=prev;
    }    
  }
  if(*to_free==(size_t)curr){
    *(to_free+1)=*(curr+1);
    *to_free=*curr;
  }
}

size_t * find_ff(size_t target) {
  size_t * curr = free_head;
  size_t curr_size;
  size_t * pre;
  while (curr != NULL) {
    curr_size = calculate_space(curr);
    if (curr_size >= target) {
      return cut_space(pre, curr, target,curr_size);
    }    
    pre = curr;
    curr = (size_t *)(*(curr + 1));
  }
  return NULL;
}

void * ff_malloc(size_t size) {
  size = get_size(size);
  size_t * res = find_ff(size);
  if (res == NULL) {
    return setup_new(size);
  }
  return (void*)res;
}

size_t * find_bf(size_t target) {
  size_t * b_pre;
  size_t *b_curr;
  size_t * pre;
  size_t * curr = free_head;
  size_t curr_size;
  size_t min_sub=INT_MAX;
  while (curr != NULL) {
    curr_size = calculate_space(curr);
    if (curr_size >= target && curr_size-target<min_sub) {
      min_sub=curr_size-target;
      b_curr=curr;
      b_pre=pre;
      if(curr_size==target){
        break;
      }
    }    
    pre = curr;
    curr = (size_t *)(*(curr + 1));
  }
  if(min_sub==INT_MAX){
    return NULL;
  }
  else{
    curr_size=calculate_space(b_curr);
    return cut_space(b_pre, b_curr, target,curr_size);
  }
  
}

void * bf_malloc(size_t size) {
  size = get_size(size);
  size_t * res = find_bf(size);
  if (res == NULL) {
    return setup_new(size);
  }
  return (void*)res;
}


void ff_free(void * ptr){
  my_free(ptr);
}
void bf_free(void * ptr) {
 my_free(ptr);
}


unsigned long get_data_segment_size() {
  return total_size;
}

unsigned long get_data_segment_free_space_size() {
  unsigned long res = 0;
  size_t * curr = free_head;
  while (curr != NULL) {
    res += *curr - (unsigned long)curr;
    curr = (size_t *)*(curr + 1);
  }
  return res;
}