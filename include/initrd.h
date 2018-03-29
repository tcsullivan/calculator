/**
 * @file initrd.h
 * Initrd image support
 * An archive file (made with ar) can be linked into the final executable to
 * allow files to be loaded in memory on boot. See mkinitrd.sh or the Makefile
 * for more info.
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

#ifndef INITRD_H_
#define INITRD_H_

#include <stdint.h>

/**
 * Structure for the archive's header.
 */
typedef struct
{
	char signature[8]; /**< The archive's signature */
} __attribute__ ((packed)) initrd_header;

/**
 * Structure for a file entry in the archive.
 */
typedef struct 
{
	char name[16];      /**< The name of the file */
	uint8_t unused[32]; /**< Unused information */
	char size[10];      /**< The file's size in bytes (as string) */
	char sig[2];        /**< A signature to start file data */
} __attribute__ ((packed)) initrd_file;

/**
 * Confirms the initrd image is loaded and valid.
 * @return non-zero if valid image found
 */
uint8_t initrd_validate(void);

/**
 * Gets contents of the given file.
 * @param name the file's name
 * @return pointer to file data, null if not found
 */
char *initrd_getfile(const char *name);

/**
 * Gets the size of the given file.
 * @param name the file's name
 * @return the file's size, in bytes
 */
uint32_t initrd_getfilesize(const char *name);

char *initrd_getnfile(unsigned int index);

#endif // INITRD_H_
