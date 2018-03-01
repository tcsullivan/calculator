/**
 * @file heap.h
 * A basic memory manager
 */

#ifndef HEAP_H_
#define HEAP_H_

#include <stdint.h>

/**
 * Initializes memory management of the given heap.
 * No overflow stuff is done, so...
 * @param buf the heap to use for allocations
 */
void heap_init(void *buf);

/**
 * Allocates a chunk of memory.
 * @param size how many bytes to claim
 * @return pointer to the allocated buffer
 */
void *malloc(uint32_t size);

/**
 * Allocates and zeros a chunk of memory.
 * @param count how many of whatever to allocate
 * @param size byte count of each whatever
 * @return pointer to the allocated buffer
 */
void *calloc(uint32_t count, uint32_t size);

/**
 * Frees the buffer allocated through malloc/calloc.
 * Please don't double-free.
 * @param the buffer to release
 */
void free(void *buf);

#endif // HEAP_H_
