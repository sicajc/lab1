#include "cache.h"
#include <assert.h>

cache_block i_cache[64][4];

uint32_t i_cache_get(uint32_t addr, uint32_t *time)
{
    //@param addr: the pc address to be read, multiple of 4 bytes
    //@param time: the cycle time taken to read the data
    // Updates the time of cycle if miss, if miss, time = time + 50 else time = time
    // Updates the cache status of i cache
    //@return the value from the mem[addr]

    // Extract the address into tag, index and offset
    // tag has 21 bits, index has 6 bits and offset has 5 bits
    uint32_t tag = addr >> 11 & 0x1FFFFF;
    uint32_t index = (addr >> 5) & 0x3F;
    uint32_t offset = (addr & 0x1F) / 4; // It is byte addressable, the smallest component is a word, 4bytes

    // Compare with all the tags with the given index
    for (int set_iter = 0; set_iter < 4; set_iter++)
    {
        // hits
        if (i_cache[index][set_iter].key.tag == tag && i_cache[index][set_iter].key.valid == 1)
        {
            // during a hit, update the LRU counter, decrement other blocks lru counter
            i_cache[index][set_iter].key.lru_cnt = 3;

            // Decrement the LRU counter for other blocks
            for (int set_iter2 = 0; set_iter2 < 4; set_iter2++)
            {
                if (set_iter2 != set_iter)
                {
                    if (i_cache[index][set_iter2].key.lru_cnt > 0)
                    {
                        i_cache[index][set_iter2].key.lru_cnt--;
                    }
                }
            }

            return i_cache[index][set_iter].value.value[offset];
        }
    }

    // Fetch the value from memory and insert it into the cache
    // Update the cache with the new value using LRU policy
    // For the given set, search for the LRU block and replace it with the new value
    // LRU block has the most recently used counter

    int least_lru_cnt_val = 3;
    int least_lru_block_num = 0;
    *time = *time + 50;

    for (int set_iter = 0; set_iter < 4; set_iter++)
    {
        // Fill in the invalid cache block first, then search for LRU block
        if (i_cache[index][set_iter].key.valid == 0)
        {
            least_lru_block_num = set_iter;
            break;
        }

        // search for the LRU block
        // Traverse the block of the set, record the number
        // of the LRU block, later replace this block with new mem block
        if (i_cache[index][set_iter].key.lru_cnt < least_lru_cnt_val)
        {
            least_lru_cnt_val = i_cache[index][set_iter].key.lru_cnt;
            least_lru_block_num = set_iter;
        }
    }

    // Replace the LRU block with the new value
    i_cache[index][least_lru_block_num].key.valid = 1;
    i_cache[index][least_lru_block_num].key.lru_cnt = 3;
    i_cache[index][least_lru_block_num].key.tag = tag;

    // gets 32 bits = 4B from memory, to fill in the whole blok, which is 32B

    for (int word_iter = 0; word_iter < 8; word_iter++)
    {
        // Fetch the data from memory, use 1234 as the value first, fetch 32bits at once
        i_cache[index][least_lru_block_num].value.value[word_iter] = inst_mem[addr/4 + word_iter];
    }

    return i_cache[index][least_lru_block_num].value.value[offset];
}