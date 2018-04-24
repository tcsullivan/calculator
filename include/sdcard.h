/**
 * @file sdcard.h
 * Provides a basic library for accessing an SD card
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

#ifndef SDCARD_H_
#define SDCARD_H_

#include <stdint.h>

/**
 * Attempts to initialize the SD card.
 * Warning: Little to no error checking is done/handled properly.
 */
void sd_init(void);

/**
 * Reads in data from the SD card.
 * LBAs are 512 bytes long.
 * @param buf the pre-allocated buffer to store data in
 * @param lba the lba to read from
 * @param count the number of bytes to read
 * @return the buffer
 */
uint8_t *sd_read(uint8_t *buf, uint32_t lba, uint32_t count);

#endif // SDCARD_H_
