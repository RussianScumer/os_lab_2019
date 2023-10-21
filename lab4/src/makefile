CC=gcc
CFLAGS=-I.

all: zombie_process_example process_memory

zombie_process_example: zombie_process_example.c
	$(CC) -o zombie_process_example zombie_process_example.c $(CFLAGS)

process_memory: process_memory.c
	$(CC) -o process_memory process_memory.c $(CFLAGS)

clean:
	rm -f zombie_process_example process_memory
