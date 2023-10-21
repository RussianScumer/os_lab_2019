CC=gcc
CFLAGS=-I.

all: zombie_process_example process_memory

zombie_process_example: zombie_process_example.c
	$(CC) -o zombie_process_example zombie_process_example.c $(CFLAGS)

process_memory: process_memory.c
	$(CC) -o process_memory process_memory.c $(CFLAGS)
parallel_sum : utils.o
	$(CC) -c sum.c -o bin/sum.o
	$(CC) -c parallel_sum.c -o parallel_sum.o -pthread 
	ar rcs bin/libsum.a bin/sum.o
	$(CC) parallel_sum.o utils.o -Lbin -lsum -o parallel_sum -pthread
sum: sum.h
	$(CC) -o sum.o -c sum.c -pthread $(CFLAGS)
clean:
	rm -f zombie_process_example process_memory
