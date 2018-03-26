/**
 * @file initrd.c
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
	initrd_file *file = (initrd_file *)((uint8_t *)initrd_start + sizeof(initrd_header));
	uint32_t offset = sizeof(initrd_header);

	while (offset < initrd_size) {
		if (initrd_nametest(file->name, name))
			return file;
		uint32_t size = initrd_getsize(file) + sizeof(initrd_file);
		offset += size;
		file = (initrd_file *)((uint8_t *)file + size + 1);
	}

	return 0;
}

char *initrd_getfile(const char *name)
{
	initrd_file *file = initrd_getfileptr(name);
	if (file == 0)
		return 0;


	char *ptr = (char *)file + sizeof(initrd_file);
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

