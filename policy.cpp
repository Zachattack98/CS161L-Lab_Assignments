#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#define BLOCK_SIZE 16

struct cache_entry {
   bool valid;  //check whether current block is empty
   uint64_t tag; //stores the tag
   uint64_t value; //stores the data
};

int main(int argc, char *argv[]) {

    //check if there are 4 inputs in a row
    if (argc < 4) {
        cout << "Invalid input parameters! Please enter associativity, cache size, and policy." << endl;
        exit(-1);
    }

   size_t assoc = stoi(argv[1]);    //associativity as first input (convert to integer)
   size_t cache_sz = stoi(argv[2]); //cache size as second input (convert to integer)
   string replace_policy(argv[3]);  //replacement algorithm as third input (with characters), i.e. LRU or FIFO
   size_t total = 0, miss = 0;      //total attempts; miss rate
      

   /* Begin implementing the cache simulator */
   size_t num_cache_blocks = cache_sz / BLOCK_SIZE;
   size_t num_sets = num_cache_blocks / assoc;  //2^index
   vector<cache_entry> cache(num_cache_blocks, {false, 0, 0});

   //initialize cache space
   /*if(cache.empty() == false) {
       for (const auto &arr : cache) {
            arr.valid = false;
            arr.tag = 0;
            arr.value = 0;
       }
   }*/

   unsigned long long read_line;

   while (cin >> hex >> read_line) {
      total++;

      //tag + index + offset = 32
      //tag -> 10 = [log2(cache_sz) - log2(BLOCK_SIZE) - log2(assoc)] + [log2(BLOCK_SIZE)]
      //offset -> log2(BLOCK_SIZE) = 4, since 2^4 = 16
      uint32_t tag   = read_line >> (log2(cache_sz) - log2(assoc)); // get tag
      uint32_t index = (read_line >> 4) & (num_sets - 1);        // get index; Note: 0x00000000000003FF -> 1024 - 1
      //uint32_t offset = log2(BLOCK_SIZE);

        // check for miss
      /*if (cache[index].tag != tag) {
         cache[index].tag = tag; //substitution
         cache[index].value;
         miss++; //increment the miss counter
      }
      else {

      }*/

      /* FIFO */
      if (replace_policy == "FIFO") {
        int first_in = 0;
        int similarF = 1;

        //filling in all empty indices with data
        //while there is still data to be transferred and empty indices still exist in the cache
        for(int i = 0; i < num_cache_blocks; i++) { //checking columns
            for (int j = 0; j < num_cache_blocks; j++) { //in case any indices in the row contains similar data
                if (cache[index].tag != tag) {
                    similarF = 1;
                }
                else {
                    similarF = 0;
                    break;
                }
            }
                
            //calculate data and miss
            if (similarF == 1) {
                cache[index].tag = tag; //substitution
                //cache[i].value = cache[index].value;
                miss++; //increment the miss counter
            }
            else {
                 cache[index].tag = tag; //substitution
            }

            if(i != 0)
                total++;
        }

        //data still needing to be transferred and no empty spaces available so we need to substitute
        for(int i = 0; i < num_cache_blocks; i++) { //checking rows
            if (cache[index].tag != tag) {
                similarF = 1;
            }
            else {
                similarF = 0;
                break;
            }
        }

        if (similarF == 1) {
            cache[first_in].tag = tag; //substitution
            //cache[i].value = cache[index].value;
            miss++; //increment the miss counter
            first_in++;
        }
        else {
            cache[first_in].tag = tag; //substitution
        }

        //reset first_in counter
        if(first_in == num_cache_blocks)
            first_in = 0;
      }
      /* END OF FIFO */

      /* LRU */
      //similar concept to FIFO, except we include metadata

      if (replace_policy == "LRU") {
        int similarL = 1;

        //filling in all empty indices with data
        //while there is still data to be transferred and empty indices still exist in the cache
        for(int i = 0; i < num_cache_blocks; i++) { //checking columns
            for (int j = 0; j < num_cache_blocks; j++) { //in case any indices in the row contains similar data
                if (cache[index].tag != tag) {
                    similarL = 1;
                }
                else {
                    similarL = 0;
                    break;
                }
            }
                
            //incrementing ALL metadata of the cache
            cache[num_cache_blocks].value += 1;

            //calculate data and miss
            if (similarL == 1) {
                cache[index].tag = tag; //substitution
                //cache[i].value = cache[index].value;
                miss++; //increment the miss counter
            }
            else {
                 cache[index].tag = tag; //substitution
            }

        }

        //data still needing to be transferred and no empty spaces available so we need to substitute
        for(int i = 0; i < num_cache_blocks; i++) { //checking rows
            if (cache[index].tag != tag) {
                similarL = 1;
            }
            else {
                similarL = 0;
                break;
            }
        }

        //incrementing ALL metadata of the cache
            cache[num_cache_blocks].value += 1;

            //calculate data and miss
            if (similarL == 1) {
                cache[index].tag = tag; //substitution
                cache[index].value = 0;
                miss++; //increment the miss counter
            }
            else {
                 cache[index].tag = tag; //substitution
            }
      }
      /* END OF LRU */
   }

    float miss_rate = (float)miss / (float)total * 100.0F;

   cout << "associativity:      " << assoc << endl;
   cout << "cache size:         " << cache_sz << endl;
   cout << "replacement policy: " << replace_policy << endl;
   cout << "miss rate:          " << fixed << setprecision(2) << miss_rate << endl;

    return 0;
}