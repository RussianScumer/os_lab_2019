#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include "utils.h"

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args) {
  int sum = 0;
  // TODO: your code here 
  return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
  /*
   *  TODO:
   *  threads_num by command line arguments
   *  array_size by command line arguments
   *	seed by command line arguments
   */

  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;
  pthread_t threads[threads_num];

  /*
   * TODO:
   * your code here
   * Generate array here
   */

  int *array = malloc(sizeof(int) * array_size);

  struct SumArgs args[threads_num];
  for (uint32_t i = 0; i < threads_num; i++) {
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args)) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  free(array);
  printf("Total: %d\n", total_sum);
  return 0;
}
/*
// sum.h
#ifndef SUM_H
#define SUM_H

#include <stdint.h>

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args);

#endif

// parallel_sum.c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "sum.h"

// Function to generate an array
void GenerateArray(int *array, uint32_t size, uint32_t seed) {
  srand(seed);
  for (uint32_t i = 0; i < size; i++) {
    array[i] = rand() % 100; // Generating a random number between 0 and 99
  }
}

int main(int argc, char **argv) {
  if (argc != 7) {
    printf("Usage: %s --threads_num <num> --seed <num> --array_size <num>\n", argv[0]);
    return 1;
  }

  uint32_t threads_num = atoi(argv[2]);
  uint32_t seed = atoi(argv[4]);
  uint32_t array_size = atoi(argv[6]);
  pthread_t threads[threads_num];
  int *array = malloc(sizeof(int) * array_size);

  GenerateArray(array, array_size, seed);

  clock_t start_time = clock();

  struct SumArgs args[threads_num];
  for (uint32_t i = 0; i < threads_num; i++) {
    args[i].array = array;
    args[i].begin = (array_size / threads_num) * i;
    args[i].end = (array_size / threads_num) * (i + 1);
    if (i == threads_num - 1) {
      args[i].end = array_size; // Ensuring the last thread processes the remaining elements
    }

    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  clock_t end_time = clock();
  double total_cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

  free(array);
  printf("Total: %d\n", total_sum);
  printf("Time taken: %f seconds\n", total_cpu_time);
  return 0;
}
*/