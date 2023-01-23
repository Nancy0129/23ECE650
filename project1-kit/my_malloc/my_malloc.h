#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#define SAVE_UNIT sizeof(size_t)
size_t * free_head = NULL;
size_t total_size = 0;
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
