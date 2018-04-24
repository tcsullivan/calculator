/*
 * @file display_text.h
 * A text-rendering/managing library to work with the display
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

#ifndef DISPLAY_TEXT_H_
#define DISPLAY_TEXT_H_

/**
 * The number of existing buffers.
 */
#define TTY_COUNT 2

/**
 * Initializes text buffers and starts the blinking cursor.
 * Must call before using other functions in this file.
 */
void text_init(void);

/**
 * Switches to the i'th buffer.
 * The display shows text from a buffer that has data and a position. By
 * switching buffers, current text (and position in the text) can be preserved
 * and switched back to later.
 * @param i the buffer to switch to
 */
void text_switch(unsigned int i);

/**
 * Puts a string of text to the display/current buffer.
 * The cursor advances, and the buffer scrolls if the end of it is reached.
 * @param s the string to put
 */
void text_puts(const char *s);

/**
 * Clears the display/current buffer.
 */
void text_clear(void);

/**
 * Sets the cursor's position.
 * @param x the x position to move to
 * @param y the y position to move to
 */
void text_setpos(uint8_t x, uint8_t y);

/**
 * Moves the cursor relative to its current position.
 * Can move in both positive and negative directions.
 * @param x how many characters to move in the x direction
 * @param y how many characters to move in the y direction
 */
void text_relpos(int8_t x, int8_t y);

#endif // DISPLAY_TEXT_H_
