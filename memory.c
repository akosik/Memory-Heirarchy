//Measuring the Memory Heirarchy
//Homework 2

#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>

uint64_t* generate_random_list(int size)
{
  uint64_t* list = (uint64_t*) malloc(sizeof(uint64_t) * size);

  int i;
  int bound = size;
  for(i = 0; i < size; i++) list[i] = rand() % bound;

  return list;
}

int main(int argc, char* argv[])
{
  if(argc != 2)
    {
      printf("Required arguments: buffer_length(N). \n");
      return -1;
    }

  //Get Arguments: size and iterations
  int size = atoi(argv[1]);
  int iters = 1000;

  //Generate seed
  srand(size);

  //Initialize Buffers
  uint64_t* buffer = generate_random_list(size);

  //Time accesses
  int i = 0;
  uint64_t access = 0;
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  for(i = 0; i < iters; ++i)
    {
      access = buffer[access];
    }
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  uint64_t elapsed = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

  if(access == 0) printf("WHAT ARE THE CHANCES?!");

  //Calculate time elapsed and time per Coord, then print
  uint64_t timePerAccess = elapsed / iters;
  printf("%llu", (long long unsigned)timePerAccess);

  free(buffer);
  buffer = NULL;

  return 0;
}
