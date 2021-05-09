#include <iostream>
#include <climits>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

#define BLOCK_SIZE 16

struct cache_entry
{
   bool valid; // whether the current block is empty
   // false is empty, true is full
   uint64_t tag; // stores the tag
};

main(int argc, char **argv)
{
   if (argc < 4)
   {
      cout << "Insufficient input argugemnts" << endl;
      exit(-1);
   }
   size_t associativity = stoi(argv[1]);
   size_t cache_size = stoi(argv[2]);     // Cache suze
   string replacement_algorithm(argv[3]); // "LRU or FIFO"
   size_t total = 0, miss = 0;

   /* functioning code block of cache simulator*/
   size_t number_of_blocks = cache_size / BLOCK_SIZE;
   vector<cache_entry> cache(number_of_blocks, {false, 0});
   size_t number_of_sets = number_of_blocks / associativity;
   // init the cache space ---> # of cache blocks, mark down
   // whether each cache block is valid ( empty), tag information of the cache

   // read memory accesses from standard input
   unsigned long long read_line;
   unsigned long long index_offset = 0;

   if (replacement_algorithm == "LRU")
   {
      while (std::cin >> std::hex >> read_line)
      {
         total++;
         uint32_t tag = read_line >> ((int)log2(cache_size) - (int)log2(associativity));
         uint32_t index = (read_line >> 4) & (number_of_sets - 1);
         // index = index & 0x000003FF;
         // need to calculate offset to find proper index
         index_offset = 1;
         for (int counter = 1; counter < ((int)log2(cache_size / BLOCK_SIZE)); counter++)
         {
            index_offset = 1 + (index_offset << 1);
         }
         // Now we have offset, so can and it with index to find specific index given inputs
         index = index & index_offset;

         // Check whether the current cache line is a hit or miss
         // in the cache data structure
         // if it is a miss: ///
         //  miss++
         //  and update the cache data structure
         // else :
         //  update the cache without incrementing miss;

         // check for miss
         if (associativity == 1)
         {
            if (cache[index].tag != tag)
            {
               cache[index].tag = tag; // replacement
               miss++;                 // increment the miss counter
            }
         }
      }
   }

   if (replacement_algorithm == "FIFO")
   {
      int first_in_tracker = 0;
      int tagCounter = 0;
      while (std::cin >> std::hex >> read_line)
      {
         total++;

         uint32_t tag = read_line >> ((int)log2(cache_size) - (int)log2(associativity));
         uint32_t index = (read_line >> 4) & (number_of_sets - 1);
         if (tagCounter < 20)
         {
            cout << tag << endl;
            cout << index << endl;
            tagCounter++;
         }

         // Check whether the current cache line is a hit or miss
         // in the cache data structure
         // if it is a miss: ///
         //  miss++
         //  and update the cache data structure
         // else :
         //  update the cache without incrementing miss;

         // check for miss
         if (associativity == 1)
         {
            if (cache[index].tag != tag)
            {
               cache[index].tag = tag; // replacement
               miss++;                 // increment the miss counter
            }
         }
         else
         {
            int i = 0;
            for (i = 0; i < associativity; i++)
            {
               if (cache[index + i].valid == false) // position is not occupied
               {
                  cout << "Filed Position" << endl;
                  miss++;
                  total++;
                  cache[index + i].tag = tag;
                  cache[index + i].valid = true;
                  // cout << cache[i + index].tag << endl;
                  break;
               }
               // otherwise current position is occupied
               if (cache[index + i].tag == tag)
               {
                  // Cache hit
                  total++;
                  break;
               }
            }
            if (i == associativity)
            {
               // Done with loop, we failed to find a match
               // and failed to find available position
               // Need replacement
               miss++;
               total++;
               cache[index + first_in_tracker].valid = true;
               //cout << "Sub happening: " << cache[first_in_tracker + index].tag << endl;
               cache[first_in_tracker + index].tag = tag;
               first_in_tracker++;
               if (first_in_tracker == associativity - 1)
               {
                  first_in_tracker = 0;
               }
            }
         }
      }
   }
   float miss_rate = (float)miss / float(total) * 100.0F;

   std::cout << "associativity:      " << associativity << std::endl;
   std::cout << "cache size:         " << cache_size << std::endl;
   std::cout << "replacement policy: " << argv[3] << endl;
   std::cout << "miss rate:          " << std::fixed << std::setprecision(2) << (miss_rate * 100.0) << std::endl;

   return 0;
}
