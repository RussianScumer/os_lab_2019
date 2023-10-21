#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>
#include <signal.h>
#include "find_min_max.h"
#include "utils.h"

pid_t* child_pids;
int pnum = -1;



int main(int argc, char **argv)
{
  int seed = -1;
  int array_size = -1;
  bool with_files = false;
  int timeout = 0;
  while (true)
  {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {"timeout", required_argument, 0, 't'},
                                      {0, 0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);
    
    if (c == -1)
      break;

    switch (c)
    {
    case 0:
      switch (option_index)
      {
      case 0:
        seed = atoi(optarg);
        if (seed <= 0)
        {
          printf("seed is a positive number\n");
          return 1;
        }
        break;
      case 1:
        array_size = atoi(optarg);
        if (array_size <= 0)
        {
          printf("array_size is a positive number\n");
          return 1;
        }
      case 2:
        pnum = atoi(optarg);
        if (pnum <= 0)
        {
          printf("pnum is a positive number\n");
          return 1;
        }
        break;
      case 3:
        with_files = true;
        break;
      case 't':
            timeout = atoi(optarg);
            if (timeout < 0) {
                printf("timeout is a positive number\n");
                return 1;
            }
            break;

      defalut:
        printf("Index %d is out of options\n", option_index);
      }
      break;
    case 'f':
      
      with_files = true;
      break;

    case '?':
      break;

    default:
      printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc)
  {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1)
  {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);
  int **pipes = malloc(sizeof(int *) * pnum);
  for (int i = 0; i < pnum; i++)
  {
    pipes[i] = (int *)malloc(sizeof(int) * 2);
    if (pipe(pipes[i]) == -1)
    {
      perror("Pipe creation failed");
      return 1;
    }
  }
  FILE **temp_files = malloc(sizeof(FILE *) * pnum);
  char** temp_files_names = malloc(sizeof(char *) * pnum);
  if (with_files)
  {
    // Создаем временные файлы для каждого дочернего процесса
    for (int i = 0; i < pnum; i++)
    {
      char filename[20];
      snprintf(filename, sizeof(filename), "temp%d.txt", i);
      temp_files[i] = fopen(filename, "w");
      temp_files_names[i] = filename;
      if (temp_files[i] == NULL)
      {
        perror("Failed to open temp file");
        return 1;
      }
    }
  }
    for (int i = 0; i < pnum; i++)
    {
      pid_t child_pid = fork();
      if (child_pid >= 0)
      {
        // successful fork
        active_child_processes += 1;
        if (child_pid == 0)
        {
          // child process
          int range_start = i * (array_size / pnum);
          int range_end = (i == pnum - 1) ? array_size : (i + 1) * (array_size / pnum);
          // parallel somehow

          if (with_files)
          {
            int range_start = i * (array_size / pnum);
            int range_end = (i == pnum - 1) ? array_size : (i + 1) * (array_size / pnum);

            // Calculate min and max in the assigned range
            struct MinMax local_min_max = GetMinMax(array, range_start, range_end);

            // Write the result to the corresponding temporary file
            fprintf(temp_files[i], "%d %d", local_min_max.min, local_min_max.max);

            // Close the temporary file
            fclose(temp_files[i]);
          }
          else
          {
            // Close unnecessary read end of the pipe
            close(pipes[i][0]);
            // Calculate min and max in the assigned range
            struct MinMax local_min_max = GetMinMax(array, range_start, range_end);

            // Write the result to the pipe
            write(pipes[i][1], &local_min_max, sizeof(struct MinMax));

            // Close the write end of the pipe
            close(pipes[i][1]);
            // use pipe here
          }
          return 0;
        }
      }
      else
      {
        printf("Fork failed!\n");
        return 1;
      }
    }
    
    while (active_child_processes > 0)
    {
      int status;
      wait(&status);
      active_child_processes -= 1;
    }
    if (timeout > 0) {
        pid_t parent_pid = getpid();
        pid_t kill_pid = fork();
        if (kill_pid == 0) {
            sleep(timeout);
            kill(parent_pid, SIGKILL);
            exit(0);
        }
    }

    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;

    for (int i = 0; i < pnum; i++)
    {
      struct MinMax local_min_max;
      if (with_files)
      {
        int min, max;
        FILE *temp_file = fopen(temp_files_names[i], "r");
        if (temp_file == NULL)
        {
          perror("Failed to open temp file123");
          return 1;
        }
        fscanf(temp_file, "%d %d", &min, &max);
        fclose(temp_file);
        local_min_max.min = min;
        local_min_max.max = max;
         // read from files
      }
      else
      {
        // read from pipes
        // Read the result from the pipe
        close(pipes[i][1]);
        read(pipes[i][0], &local_min_max, sizeof(struct MinMax));
        // Close the read end of the pipe
        close(pipes[i][0]);
      }

      if (local_min_max.min < min_max.min)
        min_max.min = local_min_max.min;
      if (local_min_max.max > min_max.max)
        min_max.max = local_min_max.max;
    }

    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);

    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;
    for (int i = 0; i < pnum; i++)
    {
      char filename[20];
      snprintf(filename, sizeof(filename), "temp%d.txt", i);
      remove(filename);
    }

    free(temp_files);
    free(temp_files_names);
    free(array);
    for (int i = 0; i < pnum; i++)
    {
      free(pipes[i]);
    }
    free(pipes);

    printf("Min: %d\n", min_max.min);
    printf("Max: %d\n", min_max.max);
    printf("Elapsed time: %fms\n", elapsed_time);
    fflush(NULL);
    return 0;
    //123
  }
