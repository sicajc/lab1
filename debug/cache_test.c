#include <stdio.h>
#include "cache.h"

u_int32_t pc,time;

cache_block i_cache[64][4];

// main function
int main() {

    // Test hit, test miss, test lru correctness
    pc = 0x00000000;
    time = 0;

    // First do I cache ,initialize the i-cache
    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 4; j++){
            i_cache[i][j].key.dirty = 0;
            i_cache[i][j].key.valid = 0;
            i_cache[i][j].key.lru_cnt = 0;
            i_cache[i][j].key.tag = 0;
            for(int k = 0; k < 8; k++){
                i_cache[i][j].value.value[k] = 0;
            }
        }
    }

    // increment the pc by 4
    for(;pc<80; pc+=4){
        // Get the value from the cache using PC
        uint32_t value = cache_get(pc,&time);

        // display time
        printf("Time: %d\n", time);
        //display the value
        printf("Value: %d\n", value);
    }



    // Get the value from the cache using PC
    uint32_t value = i_cache_get(pc,&time);

    // display time
    printf("Time: %d\n", time);
    //display the value
    printf("Value: %d\n", value);


    // cache_insert(0x1234, 0x5678);
    // Print the value
    // printf("Value: %d\n", *value);
    return 0;
}