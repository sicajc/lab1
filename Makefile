SRC = $(wildcard src/*.c)
# I want to search for every file that conatains cache
# and ends with .c
DEBUG_SRC = $(wildcard debug/*.c)
INPUT ?= $(wildcard inputs/*/*.x)

DEBUG_NAME = cache_test

CXX = g++
CXXFLAGS = -std=c++17 -isystem $(GTEST_INCLUDE) -pthread -g

.PHONY: all verify clean

# Google Test paths
GTEST_DIR = /usr/local
GTEST_LIB = $(GTEST_DIR)/lib
GTEST_INCLUDE = $(GTEST_DIR)/include

.PHONY: all verify clean

run_test: test exe_test

test: $(DEBUG_SRC)
	$(CXX) $(CXXFLAGS) $^ -lgtest -lgtest_main -o $(DEBUG_NAME)

exe_test:
	./$(DEBUG_NAME)

all: sim

sim: $(SRC)
	gcc -g -O2 $^ -o $@

basesim: $(SRC)
	gcc -g -O2 $^ -o $@


run: sim
	@python3 run.py $(INPUT)

clean:
	rm -rf *.o *~ sim
	rm -rf *.o *~ cache_test
