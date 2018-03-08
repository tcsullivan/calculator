#include <heap.h>

#define HEAP_ALIGN 4

typedef struct {
	uint32_t size;
	void *next;
} __attribute__ ((packed)) alloc_t;

static alloc_t *free_blocks;
static void *heap_end;
uint32_t heap_used;

void heap_init(void *buf)
{
	heap_end = buf;
	free_blocks = 0;
	heap_used = 0;
}

void *malloc(uint32_t size)
{
	size = (size + sizeof(alloc_t) + HEAP_ALIGN) & ~(HEAP_ALIGN - 1);

	alloc_t *node = free_blocks;
	alloc_t *prev = 0;
	while (node != 0) {
		if (node->size >= size) {
			/*if (node->size > size + sizeof(alloc_t) + HEAP_ALIGN) {
				alloc_t *rem = (alloc_t *)((uint8_t *)node +
					sizeof(alloc_t) + size);
				rem->size = node->size - size - sizeof(alloc_t);
				rem->next = node->next;
				if (prev != 0)
					prev->next = rem;
				else
					free_blocks = rem;
				node->size = size;
			} else {*/
				if (prev != 0)
					prev->next = node->next;
				else
					free_blocks = node->next;
			//}

			node->next = 0;
			heap_used += node->size;
			return (void *)((uint8_t *)node + sizeof(alloc_t));
		}

		prev = node;
		node = node->next;
	}

	node = (alloc_t *)heap_end;
	node->size = size;
	node->next = 0;

	heap_end = (void *)((uint8_t *)heap_end + size);
	heap_used += size;

	return (void *)((uint8_t *)node + sizeof(alloc_t)); 
}

void *calloc(uint32_t count, uint32_t size)
{
	uint8_t *buf = malloc(count * size);
	for (uint32_t i = 0; i < count * size; i++)
		buf[i] = 0;
	return buf;
}

void free(void *buf)
{
	if (buf == 0)
		return;

	alloc_t *alloc = (alloc_t *)((uint8_t *)buf - sizeof(alloc_t));
	heap_used -= alloc->size;
	alloc->next = free_blocks;
	free_blocks = alloc;
}
