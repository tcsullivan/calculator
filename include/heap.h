#ifndef HEAP_H_
#define HEAP_H_

#include <stdint.h>

uint32_t heap_available(void);

void *hmalloc(uint32_t size);
void *hcalloc(uint32_t count, uint32_t size);

#endif // HEAP_H_
