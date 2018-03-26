/**
 * @file flash.h
 * Provides functionality for using an external SPI flash 
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

#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>

/**
 * Initializes GPIO ports and variables for flash IO. Must be called before
 * any reads or writes.
 */
void flash_init(void);

/**
 * Does a blocking read of the flash chip into a pre-allocated buffer.
 * @param buf the buffer to read in to
 * @param addr the address to read from in the flash chip
 * @param count the number of bytes to read from the chip
 */
void flash_read(char *buf, uint32_t addr, unsigned int count);

/**
 * Does a blocking write to the flash chip using data from a pre-allocated
 * buffer.
 * @param buf the buffer to read data from
 * @param addr the address to write to in the flash chip
 * @param count the number of bytes to write to the chip
 */
void flash_write(const char *buf, uint32_t addr, unsigned int count);

#endif // FLASH_H_
