#include <heap.h>
#include <stm32l476xx.h>

#define HEAP_SIZE (16 * 1024)

static uint32_t offset = 0;

uint8_t heap[HEAP_SIZE];

void heap_init(void)
{
	// what to do...
}

uint32_t heap_available(void)
{
	return HEAP_SIZE - offset;
}

void *malloc(uint32_t size)
{
	void *alloc = &heap[offset];
	offset += size;
	return alloc;
}

void *calloc(uint32_t count, uint32_t size)
{
	/*uint32_t total = count * size;
	void *alloc = hmalloc(total);
	for (uint32_t i = 0; i < total; i++)
		((uint8_t *)alloc)[i] = 0;
	return alloc;*/

	// calloc broke
	return malloc(count * size);
}

void free(void *ptr)
{
	(void)ptr;
}
