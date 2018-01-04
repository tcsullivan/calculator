#ifndef INITRD_H_
#define INITRD_H_

#include <stdint.h>

typedef struct
{
	char signature[8];
} __attribute__ ((packed)) initrd_header;

typedef struct 
{
	char name[16];
	uint8_t unused[32];
	char size[10];
	char sig[2];
} __attribute__ ((packed)) initrd_file;

uint8_t initrd_validate(void);
char *initrd_getfile(const char *name);
uint32_t initrd_getfilesize(const char *name);

#endif // INITRD_H_
