SRC = $(wildcard src/*.c)
# I want to search for every file that conatains cache
# and ends with .c
DEBUG_SRC = $(wildcard debug/*.c)
INPUT ?= $(wildcard inputs/*/*.x)

.PHONY: all verify clean

test: $(DEBUG_SRC)
	gcc -g -O2 $^ -o cache_test

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
