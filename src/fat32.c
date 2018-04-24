/**
 * @file fat32.h
 * A very basic FAT32 reading implementation
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

#include <ctype.h>
#include <fat32.h>
#include <heap.h>
#include <sdcard.h>
#include <string.h>

static uint32_t partition_lba = 0;

static uint8_t SectorsPerCluster;
static uint32_t RootDirCluster;
static uint32_t ClustersLBA;
static uint32_t FATStartLBA;

static uint8_t *RootDir = 0;

int fat_find(void)
{
	uint8_t *block = malloc(512);

	// find FAT partition
	sd_read(block, 0, 512);
	for (unsigned int i = 450; i < 450 + 64; i += 16) {
		if (block[i] == 0x0B || block[i] == 0x0C) {
			partition_lba = *((uint32_t *)(block + i + 4));
			break;
		}
	}

	if (partition_lba == 0)
		return 0;

	// read FAT volume id
	sd_read(block, partition_lba, 512);
	SectorsPerCluster = block[0x0D];
	uint16_t ReservedSectors = *((uint16_t *)(block + 0x0E));
	uint32_t SectorsPerFAT = *((uint32_t *)(block + 0x24));
	RootDirCluster = *((uint32_t *)(block + 0x2C));

	FATStartLBA = partition_lba + ReservedSectors;
	ClustersLBA = FATStartLBA + 2 * SectorsPerFAT;

	free(block);
	return 1;
}

uint32_t fat_cluster2lba(uint32_t cluster)
{
	return ClustersLBA + (cluster - 2) * SectorsPerCluster;
}

int fat_namecmp(const char *fatname, const char *name)
{
	for (unsigned int i = 0; i < 8; i++) {
		if (name[i] == '.' || name[i] == '\0')
			break;

		if (toupper(fatname[i]) != toupper(name[i]))
			return 0;
	}

	return 1;
}

file_t *fat_findfile(const char *name)
{
	if (RootDir == 0) {
		RootDir = sd_read(malloc(512 * SectorsPerCluster),
			fat_cluster2lba(RootDirCluster), 512 * SectorsPerCluster);
	}

	uint8_t *block = RootDir;
	for (unsigned int i = 0; block[i * 32] != 0; i++) {
		if (block[i * 32] == 0xE5 || (block[i * 32 + 11] & 0x0F) == 0x0F)
			continue;

		if (fat_namecmp((char *)(block + i * 32), name)) {
			uint32_t size = *((uint32_t *)(block + i * 32 + 28));
			uint32_t start = *((uint16_t *)(block + i * 32 + 20))
				<< 16 | *((uint16_t *)(block + i * 32 + 26));
			file_t *file = malloc(sizeof(file_t));
			file->size = size;
			file->start = start;
			return file;
		}
	}

	return 0;
}

char *fat_getname(uint32_t index)
{
	if (RootDir == 0) {
		RootDir = sd_read(malloc(512 * SectorsPerCluster),
			fat_cluster2lba(RootDirCluster), 512 * SectorsPerCluster);
	}

	uint32_t idx = 0;
	uint8_t *block = RootDir;
	for (unsigned int i = 0; block[i * 32] != 0; i++) {
		if (block[i * 32] == 0xE5 || (block[i * 32 + 11] & 0x0F) == 0x0F)
			continue;

		if (idx == index) {
			char *name = strncpy(malloc(12), (char *)(block + i * 32), 11);
			name[11] = '\0';
			return name;
		}
		idx++;
	}

	return 0;
}

char *fat_readfile(file_t *file)
{
	if (file == 0)
		return 0;

	uint8_t *block = malloc(512);

	uint32_t start = file->start;
	uint8_t *buffer = malloc(file->size + 1);
	uint32_t offset = 0;
	buffer[file->size] = '\0';
	while (start != 0) {
		// find in FAT
		sd_read(block, FATStartLBA + (start / 128), 512);
		uint32_t next = ((uint32_t *)block)[start % 128];

		// read start cluster
		uint32_t size = 512 * SectorsPerCluster;
		if (file->size - offset < size)
			size = file->size - offset;
		sd_read(buffer, fat_cluster2lba(start), size);
		offset += size;

		if ((next & 0x0FFFFFFF) == 0x0FFFFFFF)
			start = 0;
		else
			start = next;

	}

	free(block);
	return (char *)buffer;
}
