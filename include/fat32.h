/**
 * @file fat32.h
 * A very basic FAT32 reading implementation
 * This only supports reading files in the root directory. Currently, the files
 * must also have a name under eight characters, with no extension.
 *
 * Copyright (C) 2018 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef FAT32_H_
#define FAT32_H_

#include <stdint.h>

/**
 * A structure to store important information for loading a file.
 */
typedef struct {
	uint32_t size;  /**< The file's size in bytes */
	uint32_t start; /**< The file's first data cluster */
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
 * Reads given file into an allocated buffer.
 * @param file the file's data structure from fat_findfile()
 * @return a malloc'd buffer containing the file's data 
 */
char *fat_readfile(file_t *file);

/**
 * Gets the name of the nth file in the root directory.
 * @param index n, the index of the file
 * @return the file's name, in a malloc'd buffer
 */
char *fat_getname(uint32_t index);

#endif // FAT32_H_
