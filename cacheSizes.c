#include <stdio.h>
#include <stdint.h>

/*
  Loops through cpuid calls. Leaf 2 CPUID didn't work on *these* computers, in polytopia, 
  so I had to use CPUID Leaf 4 and calculate the size of each buffer from the equation
  described on page 267 of the Intel 64 Software Developer's Manual.
*/

int main(int argc, char* argv[])
{

  const uint32_t msb = 0b10000000000000000000000000000000;

  FILE *f = fopen("caches.txt","w");
  int i;
  for(i = 0; i <= 3; ++i)
    {
      uint32_t eax = 0,ebx = 0,ecx = 0,edx = 0;
      __asm__("movl $4, %%eax\n\t"
	      "movl %4, %%ecx\n\t"
	      "cpuid\n\t"
	      "movl %%eax, %0\n\t"
	      "movl %%ebx, %1\n\t"
	      "movl %%ecx, %2\n\t"
	      "movl %%edx, %3\n\t"
	      :"+r"(eax),"+r"(ebx),"+r"(ecx),"+r"(edx)
	      :"r"(i)
	      :"eax","ebx","ecx","edx"
	      );
      //Cache Level
      uint32_t cachelevel = (eax >> 5) & 0x7;
      fprintf(f,"\n\nCache Level: %d.\n\n",cachelevel);
      
      //EBX = linesize, partiions, assoc
      uint32_t linesize = (ebx & 0xfff);
      uint32_t partitions = (ebx >> 12) & 0x3ff;
      uint32_t ways_of_associativity = (ebx >> 22) & 0x3ff;
      fprintf(f,"Line Size: %d\nPartitions: %d\nWays of Associativity: %d\n",linesize, partitions, ways_of_associativity);
      
      //ECX = sets
      uint32_t sets = ecx;
      fprintf(f,"Sets: %d\n",sets);
      
      //Cache Size
      uint64_t cachesize = (ways_of_associativity + 1) * (partitions + 1) * (linesize + 1) * (sets + 1);
      uint32_t cachesizeKB = cachesize / 1000;
      fprintf(f,"\t\t\t Cache Size: %llu bytes\n\t\t\t\t     %d KB\n",(long long unsigned)cachesize,cachesizeKB);
    }
  return 0;
}
