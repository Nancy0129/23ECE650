
#include "my_malloc.h"
//Turn input size to unit size
size_t get_size(size_t in_size) {
  if (in_size % SAVE_UNIT != 0) {//not a multiple of 8
    return in_size / SAVE_UNIT + 1;
  }//is a multiple of 8
  return in_size / SAVE_UNIT;
}
// Calculate a block's size
size_t calculate_space(size_t * node){
  size_t* begin=node + 2; //skip meta-data
  size_t* end=(size_t*)(*node); //the end of the block
  return (size_t)(end-begin);
}

//Create a new space on heap
void* setup_new(size_t size){
  size_t * res = sbrk((size + 2) * SAVE_UNIT);//2 extra space for meta-data
    if((void*)res==(void*)-1||res==NULL){
      return NULL;
    }
    *res = (size_t)(res + size + 2);// save the end of block in first meta-data
    total_size += (size + 2) * SAVE_UNIT; //update total size
    return (void*)(res + 2);// return the place after two meta-data

}

//Cut a large free space into adequate size to return
size_t * cut_space(size_t * pre, size_t * curr, size_t target,size_t curr_size) {  
  if (curr_size-target < 2) {  //About the same size as the target size-->directly return  
    if (curr == free_head) {
      free_head = (size_t *)(*(curr + 1));
    }
    else {
      *(pre+1) = *(curr + 1);
    }
  }
  else {// larger than the target size --> first cut, then return
  //Cut:
    *(curr + target + 2) = *curr;
    *(curr + target + 3) = *(curr + 1);
    *curr=(size_t)(curr + target + 2);
    //Delete from free block list:
    if (curr == free_head) {
      free_head = curr + target + 2;
    }
    else {     
      *(pre+1) = (size_t)(curr + target + 2);
    }    
  }
  //Return:
  return curr + 2;
}

//Free a block
void my_free(void * ptr) {
  if(ptr==NULL){// If null --> ignore
    return;
  }
  size_t * to_free = (size_t *)ptr - 2;// go to the meta-data
  if (free_head == NULL) {// set free_head
    *(to_free + 1) = (size_t)NULL;
    free_head = to_free;
    return;
  }
  size_t * curr = free_head;
  size_t * prev=NULL;
  while (curr != NULL && (size_t)curr < (size_t)to_free) {//find the place to insert
    prev = curr;
    curr = (size_t *)*(curr + 1);
  }
  if(curr==free_head){//update free_head to smaller address
    *(to_free+1)=(size_t)curr;
    free_head=to_free;
  }
  else{//insert the free block
    *(to_free+1)=(size_t)curr;
    *(prev+1)=(size_t)(to_free);
  }  
  if(*to_free==(size_t)curr){//merge to the back
    *(to_free+1)=*(curr+1);
    *to_free=*curr;
  }
  if(prev!=NULL&&*prev==(size_t)to_free){//merge to the front
      *(prev+1)=*(to_free+1);
      *prev=*to_free;
  }    
}
//Find a suitable free block according to FF
size_t * find_ff(size_t target) {
  size_t * curr = free_head;
  size_t curr_size;
  size_t * pre;
  while (curr != NULL) {
    curr_size = calculate_space(curr);
    if (curr_size >= target) {//First suitable one
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

//Find a suitable free block according to BF
size_t * find_bf(size_t target) {
  size_t * b_pre;
  size_t *b_curr;
  size_t * pre;
  size_t * curr = free_head;
  size_t curr_size;
  size_t min_sub=INT_MAX;
  while (curr != NULL) {//Find the best one
    curr_size = calculate_space(curr);
    if (curr_size >= target && curr_size-target<min_sub) {//update the best one
      min_sub=curr_size-target;
      b_curr=curr;
      b_pre=pre;
      if(curr_size==target){//if equal-->first best one-->break
        break;
      }
    }    
    pre = curr;
    curr = (size_t *)(*(curr + 1));
  }
  if(min_sub==INT_MAX){//No suitable one
    return NULL;
  }
  else{// Use the best one
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

//Go through all the free block to calculate the size
unsigned long get_data_segment_free_space_size() {
  unsigned long res = 0;
  size_t * curr = free_head;
  while (curr != NULL) {
    res += *curr - (unsigned long)curr;
    curr = (size_t *)*(curr + 1);
  }
  return res;
}