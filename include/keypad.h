/**
 * @file keypad.h
 * Manages the GPIO keypad
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

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <stdint.h>

#define K0 (uint16_t)(1 << 0)
#define K1 (uint16_t)(1 << 1)
#define K2 (uint16_t)(1 << 2)
#define K3 (uint16_t)(1 << 3)
#define K4 (uint16_t)(1 << 4)
#define K5 (uint16_t)(1 << 5)
#define K6 (uint16_t)(1 << 6)
#define K7 (uint16_t)(1 << 7)
#define K8 (uint16_t)(1 << 8)
#define K9 (uint16_t)(1 << 9)
#define KS (uint16_t)(1 << 10)
#define KP (uint16_t)(1 << 11)

/**
 * Initializes GPIO for the keypad. Must be called before any keypad reading.
 */
void keypad_init(void);

/**
 * Reads the state of the keypad and returns it.
 * @return the keypad's state
 */
uint16_t keypad_get(void);

/**
 * Tests if the given key is currently pressed, returning non-zero if it is.
 * @return non-zero if pressed
 */
uint8_t keypad_isdown(uint16_t);

#endif // KEYPAD_H_
