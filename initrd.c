#include <stdint.h>
#include <string.h>

extern uint8_t *initrd_start;
extern uint32_t initrd_size;

char *getfile(uint32_t offset)
{
	char *ptr = initrd_start;
	for (uint32_t i = 0; i < offset; i++) {
		uint32_t len = *((uint32_t *)ptr);
		uint32_t datalen = *((uint32_t *)(ptr + 4 + len));
		ptr += len + datalen + 8;
		if (ptr >= (char *)(initrd_start + initrd_size))
			return 0;
	}

	return ptr;
}

char *readfile(const char *name)
{
	char *ptr;
	for (uint32_t i = 0; ptr = getfile(i), ptr != 0; i++) {
		uint32_t len = *((uint32_t *)ptr);
		if (!strncmp(name, ptr + 4, len))
			return ptr + len + 8;
	}
	return 0;
}

uint32_t filesize(const char *name)
{
	char *ptr;
	for (uint32_t i = 0; ptr = getfile(i), ptr != 0; i++) {
		uint32_t len = *((uint32_t *)ptr);
		if (!strncmp(name, ptr + 4, len))
			return *((uint32_t *)ptr + len + 4);
	}
	return 0;
}

