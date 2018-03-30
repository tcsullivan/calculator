/**
 * @file initrd.c
 * Initrd image support
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

/**
 * The format of the initrd file is custom; made with the 'rba' utility.
 */

#include <initrd.h>
#include <string.h>

extern uint8_t _binary_initrd_img_start[];
extern uint8_t _binary_initrd_img_size[];

static const uint8_t *initrd_start = (uint8_t *)_binary_initrd_img_start;
static const uint32_t initrd_size = (uint32_t)_binary_initrd_img_size;

uint8_t initrd_validate(void)
{
	return 1; // TODO maybe add header/signature to archiver?
}

char *initrd_getfile(uint32_t offset)
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

char *initrd_readfile(const char *name)
{
	char *ptr;
	for (uint32_t i = 0; ptr = initrd_getfile(i), ptr != 0; i++) {
		uint32_t len = *((uint32_t *)ptr);
		if (!strncmp(name, ptr + 4, len))
			return ptr + len + 8;
	}
	return 0;
}

uint32_t initrd_filesize(const char *name)
{
	char *ptr;
	for (uint32_t i = 0; ptr = initrd_getfile(i), ptr != 0; i++) {
		uint32_t len = *((uint32_t *)ptr);
		if (!strncmp(name, ptr + 4, len))
			return *((uint32_t *)(ptr + len + 4));
	}
	return 0;
}

