/**
 * @file display_draw.c
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

#include <display_draw.h>
#include <display.h>
#include <task.h>
#include <clock.h>

volatile uint8_t lock = 0;
#define LOCK while (lock) { delay(5); } task_hold(1); lock = 1
#define UNLOCK task_hold(0); lock = 0

static unsigned int curx = 0;
static unsigned int cury = 0;
static unsigned int curxo = 0;
static unsigned int curyo = 0;

extern const unsigned char inconsolata24[192 * 156 * 2 + 1];

void task_cursor(void)
{
	while (1) {
		int x = curxo + curx * 12;
		int y = curyo + cury * 26;
		dsp_rect(x, y + 24, 12, 1, 0xFFFF);
		delay(300);
		dsp_rect(x, y + 24, 12, 1, 0);
		delay(300);
	}
}

void dsp_cursoron(void)
{
	task_start(task_cursor, 512);
}

void dsp_putchar(int c)
{
	LOCK;
	if (c == '\n') {
		curx = 0;
		if (++cury == 12) {
			UNLOCK;
			dsp_rect(0, 0, LCD_WIDTH, LCD_HEIGHT, 0);
			cury = 0;
		}
		UNLOCK;
		return;
	} else if (c == '\b') {
		if (curx > 0)
			curx--;
		UNLOCK;
		dsp_rect(curxo + curx * 12, curyo + cury * 26, 12, 26, 0);
		return;
	}

	unsigned int start = ((c - ' ') / 16 * 192 * 26 + (c % 16) * 12) * 2;

	unsigned int x = curxo + curx * 12;
	unsigned int y = curyo + cury * 26;
	dsp_set_addr(x, y, x + 11, y + 25);
	// for each row
	for (unsigned int i = 0; i < 26; i++) {
		// for each column
		for (int j = 12 * 2 - 1; j >= 0; j--)
			dsp_write_data(inconsolata24[start + (i * 192 * 2) + j]);
	}

	if (++curx == 40) {
		curx = 0;
		if (++cury == 12) {
			UNLOCK;
			dsp_rect(0, 0, LCD_WIDTH, LCD_HEIGHT, 0);
			LOCK;
			cury = 0;
		}
	}
	UNLOCK;
}

void dsp_puts(const char *s)
{
	unsigned int i = 0;
	while (s[i])
		dsp_putchar(s[i++]);
}

void dsp_cpos(int x, int y)
{
	curx = x;
	cury = y;
}

void dsp_coff(int x, int y)
{
	curxo = x;
	curyo = y;
}

void dsp_rect(int x, int y, int w, int h, uint16_t color)
{
	dsp_set_addr(x, y, x + w - 1, y + h - 1);
	int countdown = w * h;
	LOCK;
	do {
		dsp_write_data(color >> 8);
		dsp_write_data(color & 0xFF);
	} while (countdown--);
	UNLOCK;
}

void dsp_pixel(int x, int y, uint16_t color)
{
	LOCK;
	dsp_set_addr(x, y, x, y);
	dsp_write_data(color >> 8);
	dsp_write_data(color & 0xFF);
	UNLOCK;
}

void dsp_line(int x, int y, int i, int j, uint16_t color)
{
	int dx = i - x;
	int sx = dx >= 0 ? 1 : -1;
	int dy = j - y;
	int sy = dy >= 0 ? 1 : -1;

	if (dx < 0)
		dx *= -1;
	if (dy < 0)
		dy *= -1;
	int err = (dx > dy ? dx : -dy) / 2;
	int e2;

	while (1) {
		dsp_pixel(x, y, color);
		if (x == i && y == j)
			break;
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x += sx;
		}
		if (e2 < dy) {
			err += dx;
			y += sy;
		}
	}
}

