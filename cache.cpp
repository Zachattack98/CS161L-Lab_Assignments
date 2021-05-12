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
   vector<cache_entry> cache(number_of_blocks, {false, 0, 0, 0});
   size_t number_of_sets = number_of_blocks / associativity;
   // init the cache space ---> # of cache blocks, mark down
   // whether each cache block is valid ( empty), tag information of the cache

   // read memory accesses from standard input
   unsigned long long read_line;
   unsigned long long index_offset = 0;

   if (replacement_algorithm == "LRU")
   {
      int tagCounter = 1;
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

         vector<int> metadata_holder;

         // check for miss
         if (associativity == 1)
         {
            if (cache[index].tag != tag)
            {
               cache[index].tag = tag; // replacement
               miss++;
            }
            else
            {
               ;
            }
         }
         else
         {
            int i;

               for (i = 0; i < associativity; i++) {
                  if (cache[index + i].valid == false && cache[index + i].tag != tag) {
                  miss++;
                  cache[index + i].tag = tag;
                  cache[index + i].valid = true;
                  
                  for (int j = 0; j < associativity; j++) {
                     metadata_holder[index + j]++;  //increment all values in metadata of each index
                  }

                  if (tagCounter < 101)
                  {
                     cout << "line: " << tagCounter << ", tag: " << tag << ", index: " << index << ", missed, wrote to the " << cache[index].blockCount << "th cache block." << endl;
                  }
                  if (cache[index].blockCount < associativity - 1)
                  {
                     cache[index].blockCount++;
                  }
                  break;
               }
               else if (cache[index + i].tag == tag)
               {
                  if (tagCounter < 101)
                  {
                     cout << "line: " << tagCounter << ", tag: " << tag << ", index: " << index << ", cache hit." << endl;
                  }
                  break;
               }
            }

            //int replacementPosition;
            int fifo = 0;
            bool validMeta = 1;

            if (i >= associativity)
            {
               // Done with loop, we failed to find a match
               // and failed to find available position
               // Need replacement
               if (cache[index].FIFO_position == fifo) {
                  miss = miss + 2;
               }
               else {
                  fifo = cache[index].FIFO_position;
               }

               //replacementPosition = cache[index].FIFO_position;

               //check if all metadata indexes are equal
               for (int j=1; j < associativity; j++) {
                  if(metadata_holder[index + j - 1] != metadata_holder[index + j])
                     validMeta = 0;
               }

               if(validMeta == 1) {
                  metadata_holder[0] == 0;   //set first index to zero
               }
               else {
                  //find highest valued index in metadata
                  for (int j=1; j < associativity; j++) {
                     if(metadata_holder[index + j - 1] >= metadata_holder[index + j]) {
                        metadata_holder[index + j] = 0;
                     }
                  }
               }

               if (tagCounter < 101)
               {
                  cout << "line: " << tagCounter << ", tag: " << tag << ", index: " << index << ", missed, wrote to the " << replacementPosition << "th cache block." << endl;
                  //cout << "miss: " << miss << endl;
                  //cout << "total: " << total << endl;
               }
               cache[index].FIFO_position = (cache[index].FIFO_position + 1) % associativity;
            }
            else {
               replacementPosition = i;
            }
            
            cache[index + replacementPosition].tag = tag;
            cache[index + replacementPosition].valid = true;
            tagCounter++;
         }
      }
   }
   /************************************/

   if (replacement_algorithm == "FIFO")
   {
      int first_in_tracker = 0;
      int tagCounter = 1;
      while (cin >> hex >> read_line)
      {
         total++;

         uint32_t tag = read_line >> ((int)log2(cache_size) - (int)log2(associativity));
         uint32_t index = (read_line >> 4) & (number_of_sets - 1);

         vector<int> index_holder;

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
               miss++;  // increment the miss counter
            }
            else
            {
               ;
            }
         }
         else
         {
            int i;
            //int idx;
            //for (i = 0; i < associativity && cache[index + i].valid == false && cache[index + i].tag != tag; i++) {;}
               
               /*if ( i == associativity || cache[index+i].valid == false ) { miss++; }*/
               

               /*****if (cache[index + i].valid == false && cache[index + i].tag != tag)
               {
                  cache[index + idx].tag = tag;
                  cache[index + idx].valid = true;
               }

               cache[index].FIFO_position += 1;

               if (cache[index].FIFO_position == associativity)
               {
                  cache[index].FIFO_position = 0;
               }*****/

               /*if (i >= associativity) {
                  idx = cache[index].FIFO_position;
                  cache[index].FIFO_position = (cache[index].FIFO_position + 1) % associativity;
               }
               else{
                  idx = i;
               }

               cache[index + idx].tag = tag;
               cache[index + idx].valid = true;*/

               for (i = 0; i < associativity; i++) {
                  if (cache[index + i].valid == false && cache[index + i].tag != tag) {
                  miss++;
                  cache[index + i].tag = tag;
                  cache[index + i].valid = true;
                  if (tagCounter < 101)
                  {
                     cout << "line: " << tagCounter << ", tag: " << tag << ", index: " << index << ", missed, wrote to the " << cache[index].blockCount << "th cache block." << endl;
                  }
                  if (cache[index].blockCount < associativity - 1)
                  {
                     cache[index].blockCount++;
                  }
                  break;
               }
               else if (cache[index + i].tag == tag)
               {
                  if (tagCounter < 101)
                  {
                     cout << "line: " << tagCounter << ", tag: " << tag << ", index: " << index << ", cache hit." << endl;
                  }
                  break;
               }
            }

            int replacementPosition;
            int fifo = 0;

            if (i >= associativity)
            {
               // Done with loop, we failed to find a match
               // and failed to find available position
               // Need replacement
               if (cache[index].FIFO_position == fifo) {
                  miss = miss + 2;
               }
               else {
                  fifo = cache[index].FIFO_position;
               }

               replacementPosition = cache[index].FIFO_position;

               if (tagCounter < 101)
               {
                  cout << "line: " << tagCounter << ", tag: " << tag << ", index: " << index << ", missed, wrote to the " << replacementPosition << "th cache block." << endl;
                  //cout << "miss: " << miss << endl;
                  //cout << "total: " << total << endl;
               }
               cache[index].FIFO_position = (cache[index].FIFO_position + 1) % associativity;
            }
            else {
               replacementPosition = i;
            }
            
            cache[index + replacementPosition].tag = tag;
            cache[index + replacementPosition].valid = true;
            tagCounter++;
         }
      }
   }
   cout << "miss: " << miss << endl;
   cout << "total: " << total << endl;
   float miss_rate = (float)miss / float(total);

   cout << "associativity:      " << associativity << endl;
   cout << "cache size:         " << cache_size << endl;
   cout << "replacement policy: " << argv[3] << endl;
   cout << "miss rate:          " << fixed << setprecision(2) << (miss_rate * 100.0) << endl;

   return 0;
}