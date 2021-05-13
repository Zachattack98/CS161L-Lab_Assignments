//=========================================================================
// Name & Email must be EXACTLY as in Gradescope roster!
// Name: Zachary Hill
// Email: zhill003@ucr.edu
// 
// Assignment name: 
// Lab section: 
// TA: 
// 
// I hereby certify that I have not received assistance on this assignment,
// or used code, from ANY outside source other than the instruction team
// (apart from what was provided in the starter file).
//
//=========================================================================

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
   int FIFO_position;
   int blockCount;
};

int main(int argc, char **argv)
{
   if (argc < 4)
   {
      cout << "Insufficient input arguments less than 4" << endl;
      exit(-1); // error code:wq
   }
   size_t associativity = stoi(argv[1]);
   size_t cache_size = stoi(argv[2]);     // Cache suze
   string replacement_algorithm(argv[3]); // "LRU or FIFO"
   size_t total = 0, miss = 0;

   /* functioning code block of cache simulator*/
   size_t number_of_blocks = cache_size / BLOCK_SIZE;
   vector<cache_entry> cache(number_of_blocks, {false, 0, 0, 0});
   size_t number_of_sets = number_of_blocks / associativity;
   // init the cache space ---> # of cache blocks, mark down
   // whether each cache block is valid ( empty), tag information of the cache

   // read memory accesses from standard input
   unsigned long long read_line;
   unsigned long long index_offset = 0;

   int printCnt2 = 1;
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

         uint32_t position = index * associativity;

         vector<int> metadata_holder;  //vector for storing metadata

         int i;
         int idx;
         bool validMeta = 1;

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
            for (i = 0; i < associativity && cache[position + i].tag != tag && cache[position + i].valid == true; i++)
            {
               ;
            }

            for (int j = 0; j < associativity; j++) {
               metadata_holder[position + j]++;  //increment all values in metadata of each index
            }

            if (i >= associativity || cache[position + i].valid == false)
            {
               miss++;
               if (printCnt2 < 1000)
               {
                  cout << "line: " << printCnt2 << ", tag: " << tag << ", index: " << index << ", cache hit." << endl;

               }
            }

            // i had reached max, and no hit found;
            if (i >= associativity)
            {
               if (printCnt2 < 1000)
               {
                  cout << "Set is filled" << endl;
                  cout << "line: " << printCnt2 << ", tag: " << tag << ", index: " << index << ", missed, wrote to the " << cache[index].FIFO_position << "th cache block." << endl;
               }
               //idx = cache[position].FIFO_position;

               //check if all metadata indexes are equal
               for (int j=1; j < associativity; j++) {
                  if(metadata_holder[position + j - 1] != metadata_holder[position + j]) {
                     validMeta = 0;
                     break;
                  }
                  else {
                     validMeta = 1;
                  }
               }

               if(validMeta == 1) {
                  metadata_holder[0] == 0;   //set first index to zero
                  idx = metadata_holder[position + i];
               }
               else {
                  //find highest valued index in metadata
                  for (int j=1; j < associativity; j++) {
                     if(metadata_holder[position + j - 1] >= metadata_holder[position + j]) {
                        metadata_holder[position + j] = 0;
                        idx = metadata_holder[position + j];
                     }

                     metadata_holder[position + j] = (metadata_holder[position + j] + 1) % associativity;
                  }
               }

               //cache[position].FIFO_position = (cache[position].FIFO_position + 1) % associativity;
            }
            else
            {
               idx = i;
               // i has reached an empty slot
               if (printCnt2 < 1000)
               {
                  cout << "line: " << printCnt2 << ", tag: " << tag << ", index: " << index << ", missed, wrote to the " << cache[index].blockCount << "th cache block." << endl;
               }
                  
            }
            cache[position + idx].tag = tag;
            cache[position + idx].valid = true;
            cache[position].blockCount++;
         }
         printCnt2++;
      }
   }
   //
   int printCnt = 1;
   if (replacement_algorithm == "FIFO")
   {
      int first_in_tracker = 0;
      int tagCounter = 1;
      while (std::cin >> std::hex >> read_line)
      {
         total++;

         uint32_t tag = read_line >> ((uint32_t)log2(cache_size) - (uint32_t)log2(associativity));
         uint32_t index = (read_line >> 4) & (number_of_sets - 1);
         uint32_t position = index * associativity;   //index needs to change based on associativity to allow the correct number of misses

         int i;
         int idx;  //index to write the cache

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
            for (i = 0; i < associativity && cache[position + i].tag != tag && cache[position + i].valid == true; i++)
            {
               // Finds the value of i we need
               ;
            }
            if (i >= associativity || cache[position + i].valid == false)
            {
               miss++;
               if (printCnt < 1000)
               {
                  cout << "line: " << printCnt << ", tag: " << tag << ", index: " << index << ", cache hit." << endl;
               }
            }
            //else //since we entering this loop more often, the miss counter was higher than it should be
            //{
               // i had reached max, and no hit found;
               if (i >= associativity)
               {
                  if (printCnt < 1000)
                  {
                     cout << "Set is filled" << endl;
                     cout << "line: " << printCnt << ", tag: " << tag << ", index: " << index << ", missed, wrote to the " << cache[index].FIFO_position << "th cache block." << endl;
                  }
                  idx = cache[position].FIFO_position;
                  //cache[position].tag = tag;
                  cache[position].FIFO_position = (cache[position].FIFO_position + 1) % associativity;
               }
               else
               {
                  idx = i;
                  // i has reached an empty slot
                  if (printCnt < 1000)
                  {
                     cout << "line: " << printCnt << ", tag: " << tag << ", index: " << index << ", missed, wrote to the " << cache[index].blockCount << "th cache block." << endl;
                  }
                  
               }
               cache[position + idx].tag = tag;
               cache[position + idx].valid = true;
               cache[position].blockCount++;
            //}
         }
         printCnt++;
      }
   }
   cout << "miss: " << miss << endl;
   cout << "total: " << total << endl;
   float miss_rate = (float)miss / float(total);

   std::cout << "associativity:      " << associativity << std::endl;
   std::cout << "cache size:         " << cache_size << std::endl;
   std::cout << "replacement policy: " << argv[3] << endl;
   std::cout << "miss rate:          " << std::fixed << std::setprecision(2) << (miss_rate * 100.0) << std::endl;

   return 0;
}