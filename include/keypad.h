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

/**
 * Initializes GPIO for the keypad.
 * Starts a task to poll the buttons. Must be called before any keypad reading.
 */
void keypad_init(void);
void keypad_start(void);

/**
 * Reads the last pressed key on the keypad.
 * This driver keeps an 8 key buffer for key presses.
 * @return the pressed key (as the character it represents), zero if no presses
 */
int keypad_get(void);


#endif // KEYPAD_H_
