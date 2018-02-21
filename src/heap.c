#include <heap.h>
#include <task.h>

#define HEAP_ALIGN 16

typedef struct {
	uint32_t next;
	uint32_t size;
} __attribute__ ((packed)) alloc_t;

static alloc_t root;
static void *heap_end;

void heap_init(void *buf)
{
	heap_end = buf;
	root.next = 1;
	root.size = 0;
	// what to do...
}

void *malloc(uint32_t size)
{
	task_hold(1);
	if (size < 16)
		size = 16;
	alloc_t *node = &root;
	while (node->next & 1 || node->size < size) {
		if ((node->next & ~(1)) == 0) {
			node->next |= (uint32_t)(heap_end + HEAP_ALIGN) & ~(HEAP_ALIGN - 1);
			heap_end += 2 * HEAP_ALIGN + size;
			node = (void *)(node->next & ~(1));
			node->next = 0;
			node->size = size;
			break;
		}
		node = (void *)(node->next & ~(1));
	}

	node->next |= 1;
	task_hold(0);

	return (void *)((uint32_t)node + sizeof(alloc_t));
}

void *calloc(uint32_t count, uint32_t size)
{
	uint8_t *buf = malloc(count * size);
	for (uint8_t i = 0; i < count * size; i++)
		buf[i] = 0;
	return buf;
}

void free(void *buf)
{
	if (buf == 0)
		return;
	alloc_t *alloc = (alloc_t *)((uint32_t)buf - sizeof(alloc_t));
	alloc->next &= ~(1);
}
