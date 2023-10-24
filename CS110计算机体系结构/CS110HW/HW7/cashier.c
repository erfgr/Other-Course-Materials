#include "cashier.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//the function to find log2
uint64_t findlog(uint64_t x) {
  uint64_t y = 0;
  while (x >>= 1) {y++;}
  return y;
}

struct cashier *cashier_init(struct cache_config config) {
    /*function cashier_init: create a cache simulator with a set of parameters.
parameters: config: struct cache_config
specifies the number of bits of the address space config.address_bits.
the number of cache lines in the cache config.lines.
the size of each cache line config.line_size.
the associativity, i.e., the number of ways in the set-associative cache. config.ways.
the configuration validity is guaranteed
line_size is a power of 2
lines is a power of 2
ways is a power of 2, ways is a factor of lines.
returns: struct cashier* address to the initialized cache simulator.
note
You should return NULL on error.
You don’t need to validate the parameters.
Make sure no memory leak on error.*/
  struct cashier *cache = (struct cashier *)malloc(sizeof(struct cashier));
  if (cache == NULL) {
    return NULL; // Return NULL on allocation failure
  }
  // Initialize cache configuration
  cache->config = config;
  cache->size = config.line_size * config.lines * config.ways;
  // Calculate bit masks and number of bits
  cache->tag_bits = config.address_bits - findlog(config.lines) - findlog(config.line_size);
  cache->tag_mask = (((uint64_t)1ULL) << cache->tag_bits) - 1;
  cache->index_bits = findlog(config.lines);
  cache->index_mask = (1ULL << cache->index_bits) - 1;
  cache->offset_bits = findlog(config.line_size);
  cache->offset_mask = (1ULL << cache->offset_bits) - 1;
  // Allocate memory for cache lines
  /*
  cache->lines = (struct cache_line *)malloc(sizeof(struct cache_line) * config.lines * config.ways);
  if (cache->lines == NULL) {
    free(cache);
    return NULL; // Return NULL on allocation failure
  }
  // Initialize cache lines
  for (uint64_t i = 0; i < config.lines * config.ways; i++) {
    // Initialize cache line
    cache->lines[i].valid = false;
    cache->lines[i].dirty = false;
    cache->lines[i].tag = 0;
    cache->lines[i].last_access = 0;
    cache->lines[i].data = (uint8_t *)malloc(config.line_size);
    if (cache->lines[i].data == NULL) {
      // Free allocated memory and return NULL on allocation failure
      for (uint64_t j = 0; j < i; j++) {
        free(cache->lines[j].data);
      }
      // Free allocated memory
      free(cache->lines);
      free(cache);
      return NULL;
    }
  }
  */

  //alloc memory once
    uint8_t * mem = (uint8_t *)malloc((sizeof(struct cache_line) + config.line_size) * config.lines * config.ways);
    cache->lines = (struct cache_line *)mem;
    if (cache->lines == NULL) {
        free(cache);
        return NULL; 
        // Return NULL on allocation failure
    }

    uint8_t * data_start = mem + sizeof(struct cache_line) * config.lines * config.ways;
    for (uint64_t i = 0; i < config.lines * config.ways; i++) {
        // Initialize cache line
        cache->lines[i].valid = false;
        cache->lines[i].dirty = false;
        cache->lines[i].tag = 0;
        // Initialize cache line
        cache->lines[i].last_access = 0;
        cache->lines[i].data = data_start + i * config.line_size;}
  return cache;
}

void cashier_release(struct cashier *cache) {
  /*function cashier_release: release the resources allocated for the cache simulator.
parameters: cache: struct cashier * address to a cache simulator created by cashier_init.
returns: NONE.
note
NULL parameters is allowed.
Make sure to eliminate possibility of memory leak.*/
  if (cache == NULL) {
    return;
  }
    // Write back dirty lines
    for (uint64_t i = 0; i < cache->config.ways; i++) {
        for (uint64_t j = 0; j < cache->config.lines; j++) {
            struct cache_line *line = &cache->lines[j * cache->config.ways + i];
            if (line->valid && line->dirty) {
                // Write back the line
                uint64_t address = (line->tag << (cache->index_bits + cache->offset_bits)) | (j << cache->offset_bits);
                uint64_t index = ((address >> cache->offset_bits) & cache->index_mask) & cache->offset_mask;
                before_eviction(index, line);

                for (uint64_t offset = 0; offset < cache->config.line_size; offset++) {
                    mem_write(address + offset, line->data[offset]);
                }
            }
        }
    }

  // Write back dirty lines
//  for (uint64_t i = 0; i < cache->config.lines * cache->config.ways; i++) {
//    struct cache_line *line = &cache->lines[i];
//    if (line->valid && line->dirty) {
//      // Write back the line
//      uint64_t address = (line->tag << (cache->index_bits + cache->offset_bits)) | ((i / cache->config.ways) << cache->offset_bits);
//      for (uint64_t offset = 0; offset < cache->config.line_size; offset++) {
//        mem_write(address + offset, line->data[offset]);
//      }
//    }
//    //free(line->data); // Free memory for cache line data
//  }
  free(cache->lines); // Free memory for cache lines
  free(cache); // Free memory for cache simulator
}

bool cashier_read(struct cashier *cache, uint64_t addr, uint8_t *byte) {
  /*function cashier_read: read one byte of data at a specific address.
parameters:
cache: struct cashier * a cache simulator created by cashier_init, not NULL.
addr: uint64_t the byte address.
byte: uint8_t * used to store the result, guaranteed not to be a valid writable memory address.
returns: true on cache hit, false on cache miss.
note:
To access memory on miss, always use mem_read and mem_write. Do not directly dereference addr or SIGSEGV we be raised.
Call before_eviction if you have to evict a cache line.
Placing the cache line at a previously invalidated slot is not considered as an eviction.*/
  if (cache == NULL) {
    return false;
  }
  // Calculate tag, index, and offset
  uint64_t tag = (addr >> (cache->index_bits + cache->offset_bits)) & cache->tag_mask;
  uint64_t index = (addr >> cache->offset_bits) & cache->index_mask;
  uint64_t offset = addr & cache->offset_mask;
  for (uint64_t i = 0; i < cache->config.ways; i++) {
    struct cache_line *line = &cache->lines[index * cache->config.ways + i];
    if (line->valid && line->tag == tag) {
      // Cache hit
      line->last_access = get_timestamp();
      *byte = line->data[offset];
      return true;
    }
  }
  // Cache miss
    uint64_t evict_index = 0;
    for (uint64_t i = 0; i < cache->config.ways; i++) {
        // Find an invalid line
        if (!cache->lines[index * cache->config.ways + i].valid) {
            evict_index = i;
            break;
        }
        else {
            if(i > 0) {
                // Find the least recently used line
                uint64_t curr_timestamp = cache->lines[index * cache->config.ways + i].last_access;
                //initiate the dirty bit
                cache->lines[index * cache->config.ways + i].dirty = false;
                //uint64_t z_index = i * cache->config.ways + offset;
                //uint64_t curr_timestamp = cache->lines[z_index].last_access;
                if (curr_timestamp < cache->lines[index * cache->config.ways + evict_index].last_access) {
                    evict_index = i;
                }
            }
        }
    }
  struct cache_line *evict_line = &cache->lines[index * cache->config.ways + evict_index];
  // Call before_eviction if the evicted line is valid
  if (evict_line->valid) {
    before_eviction(index, evict_line);
    // Write back the evicted line if it is dirty
    if(evict_line->dirty) {
        uint64_t evict_address = (evict_line->tag << (cache->index_bits + cache->offset_bits)) | (index << cache->offset_bits);
        for (uint64_t zj = 0; zj < cache->config.line_size; zj++) {
            mem_write(evict_address + zj, evict_line->data[zj]);
        }
    }
  }
  // Read the line from memory
  uint64_t read_address = (tag << (cache->index_bits + cache->offset_bits)) | (index << cache->offset_bits);
  for (uint64_t zj = 0; zj < cache->config.line_size; zj++) {
    evict_line->data[zj] = mem_read(read_address + zj);
  }
  // Update the evicted line
  evict_line->valid = true;
  evict_line->dirty = false;
  evict_line->tag = tag;
  // Update the last access timestamp
  evict_line->last_access = get_timestamp();
  *byte = evict_line->data[offset];
  return false;
}

bool cashier_write(struct cashier *cache, uint64_t addr, uint8_t byte) {
  /*function cashier_write: write one byte of data at a specific address.
parameters:
cache: struct cashier * a cache simulator created by cashier_init, not NULL.
addr: uint64_t the byte address.
byte: uint8_t the data to be written into memory.
returns: true on cache hit, false on cache miss.
note:
To access memory on miss, always use mem_read and mem_write. Do not directly dereference addr or SIGSEGV we be raised.
Call before_eviction if you have to evict a cache line.
Placing the cache line at a previously invalidated slot is not considered as an eviction.
Write-back is expected rather than write-through.*/
  if (cache == NULL) {
    return false;
  }

  // Calculate tag, index, and offset
  uint64_t tag = (addr >> (cache->index_bits + cache->offset_bits)) & cache->tag_mask;
  uint64_t index = (addr >> cache->offset_bits) & cache->index_mask;
  uint64_t offset = addr & cache->offset_mask;

  for (uint64_t i = 0; i < cache->config.ways; i++) {
    struct cache_line *line = &cache->lines[index * cache->config.ways + i];

    if (line->valid && line->tag == tag) {
      // Cache hit
      line->last_access = get_timestamp();
      line->data[offset] = byte;
      line->dirty = true;
      return true;
    }
  }
  // Cache miss
  uint64_t evict_index = 0;
  for (uint64_t i = 0; i < cache->config.ways; i++) {
      if (!cache->lines[index * cache->config.ways + i].valid) {
          evict_index = i;
          break;
      }
      // else condition
      else {
          if(i > 0) {
              // Find the least recently used line
              uint64_t curr_timestamp = cache->lines[index * cache->config.ways + i].last_access;
              //uint64_t z_index = i * cache->config.ways + offset;
              //uint64_t curr_timestamp = cache->lines[z_index].last_access;
              if (curr_timestamp < cache->lines[index * cache->config.ways + evict_index].last_access) {
                  evict_index = i;
              }
          }
      }
  }
  struct cache_line *evict_line = &cache->lines[index * cache->config.ways + evict_index];
  // Call before_eviction if the evicted line is valid
  if (evict_line->valid) {
    before_eviction(index, evict_line);
    // Write back the evicted line if it is dirty
    if(evict_line->dirty) {
        uint64_t evict_address = (evict_line->tag << (cache->index_bits + cache->offset_bits)) | (index << cache->offset_bits);
        for (uint64_t zj = 0; zj < cache->config.line_size; zj++) {
            mem_write(evict_address + zj, evict_line->data[zj]);
        }
    }
  }
  //first fill the data
  uint64_t read_address = (tag << (cache->index_bits + cache->offset_bits)) | (index << cache->offset_bits);
  for (uint64_t zj = 0; zj < cache->config.line_size; zj++) {
      evict_line->data[zj] = mem_read(read_address + zj);
  }
  // Write the new data into the cache line
  evict_line->valid = true;
  evict_line->dirty = true;
  evict_line->tag = tag;
  // Write the new data into the cache line
  evict_line->last_access = get_timestamp();
  evict_line->data[offset] = byte;
  return false;
}
