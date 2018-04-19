#ifndef FAT32_H_
#define FAT32_H_

#include <stdint.h>

typedef struct {
	uint32_t size;	
	uint32_t start;
} file_t;

/**
 * Finds a FAT partition on the SD card.
 * @return non-zero if partition found
 */
int fat_find(void);

/**
 * Searches for the given file, returning necessary information to use it.
 * @param name the file's name
 * @return a file data structure, null if not found
 */
file_t *fat_findfile(const char *name);

/**
 * 
 */
char *fat_readfile(file_t *file);

char *fat_getname(uint32_t index);

#endif // FAT32_H_
