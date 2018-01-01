#include <heap.h>
#include <stm32l476xx.h>

#define RAM_END 0x20018000

//extern void *end;
//extern uint32_t _total_ram;
static uint32_t offset = 0;

__attribute__ ((section(".data")))
uint8_t heap[8192];
void *end = heap;

void heap_init(void)
{
	// what to do...
}

uint32_t heap_available(void)
{
	return 0;//	return _total_ram - offset;
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
