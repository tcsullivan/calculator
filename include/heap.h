#ifndef HEAP_H_
#define HEAP_H_

#include <stdint.h>

uint32_t heap_available(void);

void *malloc(uint32_t size);
void *calloc(uint32_t count, uint32_t size);
void free(void *buf);

#endif // HEAP_H_
