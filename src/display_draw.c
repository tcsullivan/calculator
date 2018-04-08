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
#include <flash.h>
#include <heap.h>
#include <task.h>
#include <clock.h>

#define C_WIDTH  12
#define C_HEIGHT 16
#define S_WIDTH  40
#define S_HEIGHT 20

volatile uint8_t lock = 0;
#define LOCK while (lock) { delay(5); } task_hold(1); lock = 1
#define UNLOCK task_hold(0); lock = 0

static unsigned int curx = 0;
static unsigned int cury = 0;
static unsigned int curxo = 0;
static unsigned int curyo = 0;
static unsigned char *font;

void task_cursor(void)
{
	while (1) {
		int x = curxo + curx * C_WIDTH;
		int y = curyo + cury * C_HEIGHT;
		dsp_rect(x, y + C_HEIGHT, C_WIDTH, 1, 0xFFFF);
		delay(300);
		dsp_rect(x, y + C_HEIGHT, C_WIDTH, 1, 0);
		delay(300);
	}
}

void dsp_cursoron(void)
{
	font = malloc(32 * 256);
	flash_read((char *)font, 0, 32 * 256);
	task_start(task_cursor, 512);
}

void dsp_putchar(int c)
{
	LOCK;
	if (c == '\n') {
		curx = 0;
		if (++cury == S_HEIGHT) {
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
		dsp_rect(curxo + curx * C_WIDTH, curyo + cury * C_HEIGHT,
			C_WIDTH, C_HEIGHT, 0);
		return;
	}

	unsigned int x = curxo + curx * C_WIDTH;
	unsigned int y = curyo + cury * C_HEIGHT;
	dsp_set_addr(x, y, x + C_WIDTH - 1, y + C_HEIGHT - 1);

	uint32_t base = c * 32;
	for (unsigned int j = 0; j < 16; j++) {
		uint16_t row = (font[base + j * 2] << 8) | font[base + j * 2 + 1];
		for (int i = 4; i < 16; i++) {
			uint8_t color = (row & (1 << i)) ? 0xFF : 0;
			dsp_write_data(color);
			dsp_write_data(color);
		}
	}

	if (++curx == S_WIDTH) {
		curx = 0;
		if (++cury == S_HEIGHT) {
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

