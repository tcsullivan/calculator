/**
 * @file lcd.h
 * A basic library for writing a 16x2 text LCD.
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

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

/**
 * A handler/task to manage asyncronous LCD writes.
 */
void lcd_handler(void);

/**
 * Writes a string asyncronously to the LCD.
 * The lcd_handler task must be running for the string to actually be printed.
 * @param s the string to write
 */
void lcd_put(const char *s);

//
// The following functions do not support asyncronous calls.
//

/**
 * Initializes the LCD.
 */
void lcd_init(void);

/**
 * Writes a string to the LCD.
 * A cursor position is kept internally. When the end of the screen is reached,
 * writing resumes at the first position.
 * @param s the string to write
 */
void lcd_puts(const char *s);

/**
 * Writes a base 10 integer to the screen.
 * @param i the integer to print
 */
void lcd_puti(int i);

/**
 * Writes a base 16 integer to the screen.
 * @param h the integer to print
 */
void lcd_puth(int h);

/**
 * Writes a byte in binary to the screen.
 * @param b the byte to print
 */
void lcd_putb(uint8_t b);

/**
 * Clears the LCD.
 */
void lcd_clear(void);

#endif // LCD_H_
