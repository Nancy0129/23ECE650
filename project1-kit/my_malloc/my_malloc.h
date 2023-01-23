#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
// The unit (smallest) size of each allocated block
#define SAVE_UNIT sizeof(size_t)
//The head pointer of free block list
size_t * free_head = NULL;  
//The variable to track total size
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

//Util Functions
size_t get_size(size_t in_size); //Turn input size to unit size
size_t calculate_space(size_t * node);// Calculate a block's size
void* setup_new(size_t size); //Create a new space on heap
    //Cut a large free space into adequate size to return
size_t * cut_space(size_t * pre, size_t * curr, size_t target,size_t curr_size);
void my_free(void * ptr); //Free a block
size_t * find_ff(size_t target); //Find a suitable free block according to FF
size_t * find_bf(size_t target); //Find a suitable free block according to BF