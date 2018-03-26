/**
 * @file display_draw.h
 * Provides functions for drawing to the display
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

#ifndef DISPLAY_DRAW_H_
#define DISPLAY_DRAW_H_

#include <stdint.h>

/**
 * Starts the task for a blinking text cursor.
 */
void dsp_cursoron(void);

/**
 * Sets the chosen pixel to the given color.
 * @param x x-coord of the pixel
 * @param y y-coord of the pixel
 * @param color the color to use (5-6-5)
 */
void dsp_pixel(int x, int y, uint16_t color);

/**
 * Draws a line between the two coordinates.
 * @param x start x-coord
 * @param y start y-coord
 * @param i end x-coord
 * @param j end y-coord
 * @param color the color to use (5-6-5)
 */
void dsp_line(int x, int y, int i, int j, uint16_t color);

/**
 * Draws a rectangle with the given dimensions.
 * @param x top left x-coord
 * @param y top left y-coord
 * @param w width of the rectangle
 * @param h height of the rectangle
 * @param color the color to use (5-6-5)
 */
void dsp_rect(int x, int y, int w, int h, uint16_t color);

/**
 * Sets the text cursor's position, in characters NOT pixels.
 * @param x text column to move to
 * @param y text row to move to
 */
void dsp_cpos(int x, int y);

/**
 * Sets the pixel offset of the text cursor.
 * @param x x-pixel offset from (0, 0)
 * @param y y-pixel offset from (0, 0)
 */
void dsp_coff(int x, int y);

/**
 * Puts a string to the screen. Text position if kept track of internally, so
 * multiple calls will print the strings in one line.
 * @param s the string to print
 */
void dsp_puts(const char *s);

#endif // DISPLAY_DRAW_H_
