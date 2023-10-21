#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "utils.h"
#include "sum.h"

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int main(int argc, char **argv) {

  uint32_t threads_num = atoi(argv[2]);
  uint32_t seed = atoi(argv[4]);
  uint32_t array_size = atoi(argv[6]);
  pthread_t threads[threads_num];
  int *array = malloc(sizeof(int) * array_size);
  clock_t start_time = clock();
  GenerateArray(array, array_size, seed);

   for (int i = 0; i <array_size; i++)
   {
    printf("%d\t\n", array[i]);
   }
   
  struct SumArgs args[threads_num];
  for (uint32_t i = 0; i < threads_num; i++) {
    args[i].array = array;
    args[i].begin = i*array_size/threads_num;
    args[i].end = (i+1)*array_size/threads_num;
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
  clock_t end_time = clock();
  double total_cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
  free(array);
  printf("Total: %d\n", total_sum);
  printf("Time taken: %f seconds\n", total_cpu_time);
  return 0;
}