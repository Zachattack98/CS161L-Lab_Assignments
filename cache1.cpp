#include <iostream>
#include <iomanip>
#include <vector>

static const size_t BLOCK_SIZE = 16;
static const size_t CACHE_SIZE = 16384;

/** Basically Implementing:
 *  associativity = 1;
 *  cache size = 16384 = 2^14
 *  cache address = 32 bits (tag + index + size = 32)
 *  cache block size = 16 bytes = 2^4
 *  replacement policy: FIFO
 **/

int main () {
   //build cache
   int number_of_blocks = CACHE_SIZE / BLOCK_SIZE;
   std::vector<unsigned long long> cache;

   for (int i = 0; i < number_of_blocks; ++i)
      cache.push_back(0);

   // read memory accesses from standard input
   unsigned long long read_line;
   int total = 0, miss = 0;
   while (std::cin >> std::hex >> read_line) {
      total++;
      unsigned long long tag   = read_line >> (10 + 4); // get tag
      unsigned long long index = read_line >> 4;        // get index
      index = index & 0x00000000000003FF;

      // check for miss
      if (cache[index] != tag) {
         cache[index] = tag; //substitution
         miss++; //increment the miss counter
      }
   }
   double miss_rate = 1.0 * miss / total;

   std::cout << "associativity:      " << 1 << std::endl;
   std::cout << "cache size:         " << CACHE_SIZE << std::endl;
   std::cout << "replacement policy: FIFO" << std::endl;
   std::cout << "miss rate:          " << std::fixed << std::setprecision(2) <<  (miss_rate * 100.0) << std::endl;

   return 0;
}

