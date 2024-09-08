#ifndef _CACHE_H_
#define _CACHE_H_

#define MEM_WORD_SIZE 1<<20

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

// Define d cache key, key = [dirty,valid,lru_cnt,tag] as struct
typedef struct cache_key {
    int dirty;
    int valid;
    int lru_cnt;
    u_int32_t tag;
} cache_key_t;

typedef struct cache_data_block{
    u_int32_t value[8];
}cache_data_block_t;

// Define cache block, as key and value array
typedef struct cache_block {
    cache_key_t key;
    // Each cache block is 32 bytes, number of sets
    cache_data_block_t value;
} cache_block;

//Instantiate i_cache, i_cache is a 4-way associative cache with 64 sets
extern cache_block i_cache[64][4];

//Instantiate d_cache, d_cache is a 8-way associative cache with 256 sets
// This is useful for sharing variables within the system
extern cache_block d_cache[256][8];

uint32_t  i_cache_get(u_int32_t addr,u_int32_t* time); // Returns a value and updates the i cache
uint32_t  d_cache_get(u_int32_t addr,u_int32_t* time); // Returns a value and updates the d cache

// Initialize the instruction memory
extern uint32_t inst_mem[MEM_WORD_SIZE];

void init_cache(); // Initializes the cache

#endif
