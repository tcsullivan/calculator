#include <heap.h>
#include <stm32l476xx.h>

#define RAM_END 0x20018000

#define HEAP_SIZE (16 * 1024)

static uint32_t offset = 0;

__attribute__ ((section("._user_heap_stack")))
uint8_t heap[HEAP_SIZE];
void *end = heap;

void heap_init(void)
{
	// what to do...
}

uint32_t heap_available(void)
{
	return HEAP_SIZE - offset;
}

void *hmalloc(uint32_t size)
{
	void *alloc = end + offset;
	offset += size;
	return alloc;
}

void *hcalloc(uint32_t count, uint32_t size)
{
	uint32_t total = count * size;
	void *alloc = hmalloc(total);
	for (uint32_t i = 0; i < total; i++)
		((uint8_t *)alloc)[i] = 0;
	return alloc;
}
