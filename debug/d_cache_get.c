#include "cache.h"
#include <assert.h>

cache_block d_cache[256][8];

uint32_t d_cache_get(int read, uint32_t data, uint32_t addr, uint32_t *time)
{
    // Extract the address into tag, index and offset
    // tag has 19 bits, index has 8 bits and offset has 5 bits
    uint32_t tag = addr >> 13 & 0x7FFFF;
    uint32_t index = (addr >> 5) & 0xFF;
    uint32_t offset = (addr & 0x1F) / 4; // It is byte addressable, the smallest component is a word, 4bytes

    // assert tags, index and offset
    assert(tag < 0x7FFFF && index < 0xFF && offset < 0x1F / 4);

    // cache hit
    // Compare with all the tags with the given index
    for (int set_iter = 0; set_iter < 8; set_iter++)
    {
        // hits
        if (d_cache[index][set_iter].key.tag == tag && d_cache[index][set_iter].key.valid == 1)
        {
            // during a hit, update the LRU counter, decrement other blocks lru counter
            d_cache[index][set_iter].key.lru_cnt = 7;
            // Decrement the LRU counter for other blocks
            for (int set_iter2 = 0; set_iter2 < 8; set_iter2++)
            {
                if (set_iter2 != set_iter)
                {
                    if (d_cache[index][set_iter2].key.lru_cnt > 0)
                    {
                        d_cache[index][set_iter2].key.lru_cnt--;
                    }
                }
            }

            if (read == 0)
            { // write
                // write to the cache
                d_cache[index][set_iter].value.value[offset] = data;
                // mark the dirt bit
                d_cache[index][set_iter].key.dirty = 1;
            }
            else // read
            {
                return d_cache[index][set_iter].value.value[offset];
            }
        }
    }

    // cache miss
    int least_lru_cnt_val = 7;
    int least_lru_block_num = 0;
    *time = *time + 50;

    // search for the lru block
    for (int set_iter = 0; set_iter < 8; set_iter++)
    {
        // search for the LRU block
        // Traverse the block of the set, record the number
        // of the LRU block, later replace this block with new mem block
        if (d_cache[index][set_iter].key.lru_cnt < least_lru_cnt_val)
        {
            least_lru_cnt_val = d_cache[index][set_iter].key.lru_cnt;
            least_lru_block_num = set_iter;
        }
    }

    if (read == 0) // write
    {
        // Replace the LRU block with the new value
        d_cache[index][least_lru_block_num].key.valid = 1;
        d_cache[index][least_lru_block_num].key.lru_cnt = 7;
        d_cache[index][least_lru_block_num].key.tag = tag;

        // write to the cache
        d_cache[index][least_lru_block_num].value.value[offset] = data;
        d_cache[index][least_lru_block_num].key.dirty = 1;
    }
    else // read
    {
        // Replace the LRU block with the new value
        d_cache[index][least_lru_block_num].key.valid = 1;
        d_cache[index][least_lru_block_num].key.lru_cnt = 7;
        d_cache[index][least_lru_block_num].key.tag = tag;

        if(d_cache[index][least_lru_block_num].key.dirty == 1)
        {
            // write to memory

        }

        // read to the cache from memory
        for (int word_iter = 0; word_iter < 8; word_iter++)
        {
            // Fetch the data from memory, use 1234 as the value first, fetch 32bits at once
            i_cache[index][least_lru_block_num].value.value[word_iter] = 0x12341234;
        }

        d_cache[index][least_lru_block_num].key.dirty = 0;

        return d_cache[index][least_lru_block_num].value.value[offset];
    }
}