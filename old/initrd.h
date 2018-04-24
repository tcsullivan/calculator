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
 * Confirms the initrd image is loaded and valid.
 * @return non-zero if valid image found
 */
uint8_t initrd_validate(void);

/**
 * Gets the file name of the index'th file in the archive.
 * @param index the index of the file
 * @return the file's name, or zero if not found
 */
char *initrd_getfile(uint32_t index);
char *initrd_getname(uint32_t index);

/**
 * Gets contents of the given file.
 * @param name the file's name
 * @return pointer to file data, null if not found
 */
char *initrd_readfile(const char *name);

/**
 * Gets the size of the given file.
 * @param name the file's name
 * @return the file's size, in bytes
 */
uint32_t initrd_filesize(const char *name);

#endif // INITRD_H_
