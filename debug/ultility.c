#include "cache.h"
#include <assert.h>

void init_cache()
{
    // Initialize the cache
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            i_cache[i][j].key.dirty = 0;
            i_cache[i][j].key.valid = 0;
            i_cache[i][j].key.lru_cnt = 0;
            i_cache[i][j].key.tag = 0;
            for (int k = 0; k < 8; k++)
            {
                i_cache[i][j].value.value[k] = 0;
            }
        }
    }

    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            d_cache[i][j].key.dirty = 0;
            d_cache[i][j].key.valid = 0;
            d_cache[i][j].key.lru_cnt = 0;
            d_cache[i][j].key.tag = 0;
            for (int k = 0; k < 8; k++)
            {
                d_cache[i][j].value.value[k] = 0;
            }
        }
    }

}
