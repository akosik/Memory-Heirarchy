//Measuring the Memory Heirarchy
//Homework 2

#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>


//Credit to Stack Exchange User Ryan Reich for this modification to rand
//Works a little better than just using modulo
uint64_t random_at_most(uint64_t max) {
  uint64_t
    num_bins = (uint64_t) max + 1,
    num_rand = (uint64_t) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  uint64_t x;
  do {
   x = random();
  }
  while (num_rand - defect <= (uint64_t)x);

  return x/bin_size;
}

uint64_t* generate_random_list(uint64_t size)
{
  uint64_t* list = (uint64_t*) malloc(sizeof(uint64_t) * size);
  if(list == NULL) exit(1);

  int i;
  uint64_t bound = size;
  for(i = 0; i < size; i++) list[i] = random_at_most(bound);
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
  const uint64_t size = atoi(argv[1]);
  const uint64_t iters = 1000;

  //Generate seed
  srandom(time(NULL));

  //Initialize Buffers
  uint64_t* buffer = generate_random_list(size);

  //Time accesses
  uint64_t i = 0;
  uint64_t access = random() % size;
  uint64_t elapsed;
  struct timespec start, end;
  FILE *f = fopen("accesses.txt","w+");
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  for(;i < iters; ++i) 
    {
      access = buffer[access];
    }
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  elapsed = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

  fprintf(f,"%llu\n",(long long unsigned)elapsed);

  if(access == 0) printf("WHAT ARE THE CHANCES?!");

  //Calculate time elapsed and time per Coord, then print
  uint64_t timePerAccess = elapsed / iters;
  printf("%llu", (long long unsigned)timePerAccess);

  free(buffer);
  buffer = NULL;

  return 0;
}
