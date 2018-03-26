/*
 * @file display.h
 * Display library for ILI9481 display
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

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

/**
 * The screen's width, in pixels.
 */
#define LCD_WIDTH  480

/**
 * The screen's height, in pixels.
 */
#define LCD_HEIGHT 320

/**
 * Initializes the display. Must be called before other display use.
 */
void dsp_init(void);

/**
 * Returns the color integer for the given RGB values.
 * Converts 8RGB to 5-6-5.
 * @param r red value, 0-255
 * @param g green value, 0-255
 * @param b blue value, 0-255
 * @return the 5-6-5 color value
 */
uint16_t dsp_color(uint8_t r, uint8_t g, uint8_t b);

/**
 * Sets the direction of IO, for reading or writing to the screen.
 * @param mode INPUT or OUPUT (defined in gpio.h)
 */
void dsp_dmode(int mode);

/**
 * Writes the command byte to the display.
 * @param data the command to write
 */
void dsp_write_cmd(uint8_t data);

/**
 * Writes the data byte to the display.
 * @param data the data to write
 */
void dsp_write_data(uint8_t data);

/**
 * Reads a byte of data from the display.
 * @return the data byte
 */
uint8_t dsp_read_data(void);

/**
 * Selects the area of pixels to draw to, from (x1, y1) inclusive to (x2, y2)
 * exclusive.
 * @param x1 starting x coordinate
 * @param y1 starting y coordinate
 * @param x2 ending x coordinate
 * @param y2 ending y coordinate
 */
void dsp_set_addr(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * Selects the area of pixels to read from, in the range (x1, y1) inclusive to
 * (x2, y2) exclusive.
 * @param x1 starting x coordinate
 * @param y1 starting y coordinate
 * @param x2 ending x coordinate
 * @param y2 ending y coordinate
 */
void dsp_set_addr_read(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

#endif // DISPLAY_H_
