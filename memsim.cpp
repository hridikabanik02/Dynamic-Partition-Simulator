//Name: Hridika Banik
//UCID: 30123716

/// -------------------------------------------------------------------------------------
/// this is the only file you need to edit
/// -------------------------------------------------------------------------------------
///
/// (c) 2022, Pavol Federl, pfederl@ucalgary.ca
/// Do not distribute this file.


#include "memsim.h"
#include <cassert>
#include <iostream>
#include <iomanip>
#include <list>
#include <set>
#include <iterator>
#include <unordered_map>
#define SPACES 20
using namespace std;


//This function has been adapted from Stephane Dorotich hint code:
// https://github.com/stephanedorotich/t457/blob/main/7-Memsim/memsim.cpp

struct Partition {
    int tag; 
    int64_t size, addr;
    Partition(int tag, int64_t size, int64_t addr) {
        this->tag = tag;
        this->size = size;
        this->addr = addr;
    }
};
typedef std::list<Partition>::iterator PartitionRef;


struct scmp {
    bool operator()(const PartitionRef & c1, const PartitionRef & c2) const {
        if(c1->size == c2->size) 
            return c1->addr < c2->addr; 
        else
            return c1->size > c2->size; 
    }
};

// I recommend you implement the simulator as a class. This is only a suggestion.
// If you decide not to use this class, feel free to remove it.

struct Simulator {

    
    MemSimResult result;
    int64_t page_size;

    // Step 1
    // all partitions, in a linked list
    list<Partition> all_blocks;

    // Step 2
    // quick access to all tagged partitions
    unordered_map<long, vector<PartitionRef>> tagged_blocks; 

    // Step 3
    // sorted partitions by size/address
    set<PartitionRef, scmp> free_blocks; 

    //constructor
    Simulator(int64_t page_size) {
        this->page_size = page_size;
        result.n_pages_requested = 0;
        result.max_free_partition_size = 0;
        result.max_free_partition_address = 0;
    }


    //This function has been adapted from Stephane Dorotich hint code:
    // https://github.com/stephanedorotich/t457/blob/main/7-Memsim/memsim.cpp

    PartitionRef get_largest_free_block() {
    /*Good for Step 1
    Good for Step 2
    Update for Step 3

    - search through the list of partitions from start to end, and
      find the largest partition that fits requested size
        - in case of ties, pick the first partition found
    - if no suitable partition found:
        - return all_blocks.end()

    Good for Step 1
    Good for Step 2
    Update for Step 3
    for (PartitionRef it = all_blocks.begin(); it != all_blocks.end(); it=std::next(it)) {
      find largest free block
    }

    if no blocks found || largest free block is too small for request
    return all_blocks.end();
    otherwise, return largest free block*/


    if(free_blocks.empty() != 0)
        return all_blocks.end(); //no space is free
    else
        return *(free_blocks.begin());
    }


    //This function has been adapted from Stephane Dorotich hint code:
    // https://github.com/stephanedorotich/t457/blob/main/7-Memsim/memsim.cpp
    PartitionRef create_new_block(int size, PartitionRef partition) {
        // Good for Step 1
        // Good for Step 2
        // Update for Step 3

        // - if no suitable partition found:
        //     - get minimum number of pages from OS, but consider the
        //       case when last partition is free
        //     - add the new memory at the end of partition list
        //     - the last partition will be the best partition

        //we enter this when there are no blocks and we need a new block
        if(all_blocks.empty() == 1) {
            int pgrequest = 1;  //this is default as there should be at least one page
            int updated_size = size - 1;
            pgrequest = pgrequest + ((updated_size) / page_size); //this is the number of pages requested
            int blocks_req = page_size * pgrequest; // determines the size that was got
            result.n_pages_requested = result.n_pages_requested + pgrequest; 

            all_blocks.emplace_back(-1, blocks_req, 0);
            free_blocks.insert(all_blocks.begin()); //new partition is added to free_blocks  list
        }
        //we enter this if blocks are not empty and we need add a new block at the end
        else if (all_blocks.empty() != 1){

            int pgrequest; //this is the number of pages requested
            int blockrequest; //this is the number of blocks requested
        

            //if the tag is free which means if it is -1
            if(all_blocks.back().tag == -1) {


                partition = all_blocks.end();
                partition = prev(partition); //decrementing
                free_blocks.erase(partition); //to add partition to set again, we need to remove it first and then update it
                pgrequest = 1;  //this is default as there should be at least one page
                int updatedsize = size - all_blocks.back().size - 1;
                pgrequest = pgrequest + ((updatedsize) / page_size); //integer division gives rounded up integer
                blockrequest = page_size * pgrequest; // determines the size that was got
                int x = all_blocks.back().size;
                all_blocks.back().size = x + blockrequest; 
                free_blocks.insert(partition);

            }

            //if tag is not -1
            else if (all_blocks.back().tag != -1) {
                pgrequest = 1;
                int updated_size = size - 1;
                pgrequest = pgrequest + ((updated_size) / page_size); //integer division gives rounded up integer
                blockrequest = page_size * pgrequest; // determines the size that was got
                all_blocks.emplace_back(-1, blockrequest, (all_blocks.back().addr + all_blocks.back().size)); // adds the new memory at the end of partition list
                free_blocks.insert(prev(all_blocks.end())); 
                //taking the last value of all_blocks vector and setting it as partition value
                partition = all_blocks.end();
                partition = prev(partition); //decrementing
                
            }
            result.n_pages_requested = result.n_pages_requested + pgrequest; 
        }
        return prev(all_blocks.end()); // correct
    }

    


    //This function has been adapted from Stephane Dorotich hint code:
    // https://github.com/stephanedorotich/t457/blob/main/7-Memsim/memsim.cpp
    void allocate(int tag, int size) {
        
        /*Good for Step 1
        Good for Step 2
        Good for Step 3
        PartitionRef p = get_largest_free_block(size);
        if (p == all_blocks.end()) // no suitable partition found, create a new block
        p = create_new_block(size);
            
        Update for Step 1
        Update for Step 2
        Update for Step 3
        - split the best partition in two if necessary
            - mark the first partition occupied, and store the tag in it
            - mark the second partition free

        Use insert() from cpp list*/

 
        PartitionRef p = get_largest_free_block();
        
        //if the size is not enough
        if(size > p->size ) {
            p = create_new_block(size, p);
           
        }
        
        else if (p == all_blocks.end()){  //if there is no free space at all
            p = create_new_block(size, p);
        }
        
        if((size == p->size) == 1) { 
        
            free_blocks.erase(p); 
            p->tag = tag;
            tagged_blocks[tag].push_back(p);


        }


        else if((size < p->size) == 1) {
            
        
            all_blocks.insert(p, Partition(tag, size, p->addr));  //new partition has been inserted
            free_blocks.erase(p);
            p->addr = p->addr + size;
            p->size = p->size - size;
            free_and_merge(p);

            tagged_blocks[tag].push_back(prev(p)); 
            free_blocks.insert(p); //free partition is inserted again
        }
    }




    //This function has been adapted from Stephane Dorotich hint code:
    //https://github.com/stephanedorotich/t457/blob/main/7-Memsim/memsim.cpp

    //This is where deallocation is done
    void deallocate(int tag) {

        for(PartitionRef p : tagged_blocks[tag]) {
            
            while((p->tag == tag)== 1) {
                free_and_merge(p);
            

            }
        }
        tagged_blocks[tag].clear();
    }

    void free_and_merge(PartitionRef p) {
        PartitionRef previous;
        PartitionRef n;
        
        previous = prev(p);
        n = next(p);
        p->tag = -1; //free is labelled by -1 


                    if((p == all_blocks.begin()) == 0) { 
                        if((previous->tag == -1) == 1) {
                            p->size = p->size + previous->size; 
                            p->addr = previous->addr; //address is updated
                            free_blocks.erase(previous); //erased from the free block set
                            all_blocks.erase(previous); 
                        }
                    }
                    

                    if((n == all_blocks.end()) == 0) { 
                        if(n->tag == -1) {
                            p->size = p->size +  n->size; 
                            free_blocks.erase(n); 
                            all_blocks.erase(n); 
                        }
                    }
                    free_blocks.insert(p); //inserted into free_blocks
    }
    MemSimResult getStats() {

        // Get largest_free_partition_size
        // Get largest_free_partition_address

        // result.largest_free_partition_size = ???
        // result.largest_free_partition_address = ???

        //we only get this information if all blocks is not empty
        if(all_blocks.empty() == 0) {
            //iterating through all the blocks and add partition size and address
            //we will get .max_free_partition_size and max_free_partition_address
            for(PartitionRef p = all_blocks.begin(); p != all_blocks.end(); p = next(p)) {
                if(p->tag == -1) {
                    if ((result.max_free_partition_size < p->size) == 1){

                        result.max_free_partition_size = p->size;
                        result.max_free_partition_address = p->addr;
                    }
                }
            }
        }
        return result;
    }





};

// re-implement the following function
// ===================================
// parameters:
//        page_size: integer in range [1..1,000,000]
//        requests: array of requests
MemSimResult mem_sim(int64_t page_size, const std::vector<Request> & requests)
{
    Simulator sim(page_size);
    for (const auto & req : requests) {
        if (req.tag < 0) {
            sim.deallocate(-req.tag);
        } else {
            sim.allocate(req.tag, req.size);
        }
    }
    return sim.getStats();
}