#include <stdio.h>
#include <gtest/gtest.h>
#include "cache.h"

// instantiate memory
uint32_t inst_mem[MEM_WORD_SIZE];

struct cache_test : testing::Test
{
    int NUM_OF_TESTS = 100;

    void SetUp()
    {
        // Initialize the cache
        init_cache();

        // Seed the random number generator
        srand(1234);

        // randomy generate the init_mem
        for (int i = 0; i < MEM_WORD_SIZE; i++)
        {
            inst_mem[i] = rand();
        }
    }

    void TearDown()
    {
        //
    }
};

TEST_F(cache_test, TestHitOf2DataOfSameBlockI_cache)
{
    // Testing strategy:
    // Insert pc address 0,4,8,12,16,20,24,28,0,4,8,12,16,20,24,28,32.... repeatly into the cache
    // The cache is 4-way associative, 64 sets, 32B block size
    // Expect the hit rate to be 100%, as the same block is being accessed

    uint32_t cycle_time = 0;
    uint32_t addr = 0;
    for (int i = 0; i < NUM_OF_TESTS; i++)
    {
        addr = addr % 32;

        uint32_t golden = inst_mem[addr];
        uint32_t value  = i_cache_get(addr, &cycle_time);

        ASSERT_EQ(value, golden) << "Num of tests: " << i << "  Addrs:" << addr;

        addr += 4;
    }

    // Expect the time to be 0
    // (cycle_time, 50);
    ASSERT_EQ(cycle_time, 50);

    // Expects reading the same value from the cache
}

TEST_F(cache_test, TestMissOf2DataOfDifferentBlockI_cache)
{
    // Testing strategy:
    // Insert PC address of (tag,index) pair to test execution of same set but with different data
    // The cache is 4-way associative, 64 sets, 32B block size
    // Expect the hit rate to be 0%, as the different block is being accessed

    uint32_t cycle_time = 0;
    uint32_t addr = 4;
    for (int i = 0; i < 100; i++)
    {
        addr = addr % 32;
        uint32_t value = i_cache_get(addr, &cycle_time);
        // EXPECT_EQ(value, 0x12341234);
        addr += 4;
    }

    // Expect the time to be 50
    EXPECT_EQ(cycle_time, 100);
}


int main() {
	// Your code here
    testing::InitGoogleTest();
    RUN_ALL_TESTS();

	return 0;
}
