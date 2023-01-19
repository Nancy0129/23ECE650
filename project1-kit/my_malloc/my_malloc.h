#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SAVE_UNIT sizeof(size_t)
size_t * free_head = NULL;
// Padding the input size to the multiple of save unit
//size_t padding(size_t in_size);

//First Fit malloc/free
void * ff_malloc(size_t size);
void ff_free(void * ptr);
//Best Fit malloc/free
void * bf_malloc(size_t size);
void bf_free(void * ptr);

//Performance
unsigned long get_data_segment_size();             //in bytes
unsigned long get_data_segment_free_space_size();  //in byte

//Functions
size_t get_size(size_t in_size);
