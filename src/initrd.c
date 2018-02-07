#include <initrd.h>

extern uint8_t _binary_initrd_img_start[];
extern uint8_t _binary_initrd_img_size[];

static const void *initrd_start = (void *)_binary_initrd_img_start;
static const uint32_t initrd_size = (uint32_t)_binary_initrd_img_size;

static const char *initrd_sig = "!<arch>\n";

uint8_t initrd_validate(void)
{
	initrd_header *header = (initrd_header *)initrd_start;
	for (uint8_t i = 0; i < 8; i++) {
		if (header->signature[i] != initrd_sig[i])
			return 0;
	}

	return 1;
}

uint8_t initrd_nametest(char *file, const char *want)
{
	for (uint8_t i = 0; i < 16; i++) {
		if (want[i] == '\0')
			return (file[i] == '/');
		else if (want[i] != file[i])
			return 0;
	}

	return 0;
}

uint32_t ipow10(uint8_t n)
{
	uint32_t i = 1;
	while (n--)
		i *= 10;
	return i;
}

uint32_t initrd_getsize(initrd_file *file)
{
	uint32_t size = 0;
	char *p = file->size + 10;
	while (*--p == ' ');

	for (int8_t i = p - file->size, j = 0; i >= 0; i--, j++)
		size += (*p-- - '0') * ipow10(j);

	return size;
}

initrd_file *initrd_getfileptr(const char *name)
{
	initrd_file *file = (initrd_file *)(initrd_start + sizeof(initrd_header));
	uint32_t offset = sizeof(initrd_header);

	while (offset < initrd_size) {
		if (initrd_nametest(file->name, name))
			return file;
		uint32_t size = initrd_getsize(file) + sizeof(initrd_file);
		offset += size;
		file += size;
	}

	return 0;
}

char *initrd_getfile(const char *name)
{
	initrd_file *file = initrd_getfileptr(name);
	if (file == 0)
		return 0;


	char *ptr = (char *)((void *)file + sizeof(initrd_file));
	ptr[initrd_getsize(file) - 1] = 0;
	return ptr;
}

uint32_t initrd_getfilesize(const char *name)
{
	initrd_file *file = initrd_getfileptr(name);
	if (file == 0)
		return 0;

	return initrd_getsize(file);
}

