
#include "my_malloc.h"

size_t get_size(size_t in_size) {
  if (in_size % SAVE_UNIT != 0) {
    return in_size / SAVE_UNIT + 1;
  }
  return in_size / SAVE_UNIT;
}
void * cut_space(size_t * pre, size_t * curr, size_t target,size_t curr_size) {
  if (curr_size-target < 2) {
    if (curr == free_head) {
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
    if (curr == free_head) {
      free_head = curr + target + 1;
    }
    else {     
      *(pre+1) = (size_t)(curr + target + 1);
    }    
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
    if((void*)res==(void*)-1){
      return NULL;
    }
    *res = (size_t)(res + size + 1);
    total_size += (size + 1) * SAVE_UNIT;
    return res + 1;
  }
  return res;
}

void ff_free(void * ptr) {
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
    if (curr == free_head) {
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
    if(curr==free_head){
      free_head=to_free;
    }
    else if(prev!=to_free){
      *(prev+1)=(size_t)to_free;
    }
  }
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

// For testing
// int main(void){
// void* start=sbrk(0);
// void* med = sbrk(20);
// void* end;
// end=sbrk(0);
// printf("%lu\n",(size_t)end);
// end=sbrk(0);
// printf("%lu\n",(size_t)end);


// size_t expected=(size_t)start;
// size_t actual;
// size_t usable=0;
// int* a[10];
// size_t addsave[10];
// for(int i=1;i<=10;i++){
//    a[i-1]= (int*)ff_malloc(i*sizeof(int));
//    expected+=i*sizeof(int)+sizeof(size_t*);
//    for(int j=0;j<i;j++){
//     a[i-1][j]=1;
//    }
// }
// int sum=0;
// for(int i=0;i<10;i++){
//   for(int j=0;j<=i;j++){
//     sum+=a[i][j];
//   }
// }
// for(int i=0;i<3;i++){
//   ff_free(a[i]);
// }
// end=sbrk(0);
// addsave[0]=(size_t)end;
// int * new_ptr[2];
// new_ptr[0]=(int*)ff_malloc(2*sizeof(int));
// end=sbrk(0);
// addsave[1]=(size_t)end;
// ff_free(a[3]);
// new_ptr[1]=(int*)ff_malloc(8*sizeof(int)-sizeof(size_t*));
// end=sbrk(0);
// addsave[2]=(size_t)end;
// ff_free(new_ptr[0]);
// ff_free(new_ptr[1]);
// for(int i=4;i<10;i++){
//   ff_free(a[i]);
// }
// size_t int_size=sizeof(int);
// void * final_pointer=ff_malloc(55*sizeof(int)+9*sizeof(size_t*));
// end=sbrk(0);
// addsave[3]=(size_t)end;
// ff_free(final_pointer);
// unsigned long empty_size= get_data_segment_free_space_size();
// end=sbrk(0);
// printf("start pointer:  %lu\n",(size_t)start);
// printf("Sum result: %d \n",sum);
// printf("Empty size:  %lu Int size %lu\n",empty_size,int_size);
// for(int i=0;i<4;i++){
//   printf("Save %d is %lu\n",i,addsave[i]);
// }
// printf("Expected Address: %lu, Actual Address: %lu\n",expected,(size_t)end); 
// printf("Calculated size: %lu  Actual size: %lu\n",total_size,(size_t)end-(size_t)start);
// return 0;

// }

void bf_free(void * ptr) {
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
void * bf_malloc(size_t size) {
  size = get_size(size);
  size_t * res = find_ff(size);
  if (res == NULL) {
    res = sbrk((size + 1) * SAVE_UNIT);
    if((void*)res==(void*)-1){
      return NULL;
    }
    *res = (size_t)(res + size + 1);
    total_size += (size + 1) * SAVE_UNIT;
    return res + 1;
  }
  return res;
}
