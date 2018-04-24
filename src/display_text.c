/*
 * @file display_text.c
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

#include <clock.h>
#include <task.h>
#include <display_draw.h>
#include <heap.h>
#include <string.h>

#define WIDTH  40
#define HEIGHT 18

typedef struct {
	char *buf;
	uint8_t x;
	uint8_t y;
} tty_t;

static tty_t text_tty[TTY_COUNT];
static tty_t *text_current;
static uint8_t text_cursor;

void task_cursor(void)
{
	while (1) {
		delay(300);
		if (text_cursor == 0)
			continue;

		int x = text_current->x * C_WIDTH;
		int y = text_current->y * C_HEIGHT;
		dsp_rect(x, y + C_HEIGHT - 1, C_WIDTH, 1, 0xFFFF);
		delay(300);
		dsp_rect(x, y + C_HEIGHT - 1, C_WIDTH, 1, 0);
	}
}

void text_init(void)
{
	for (int i = 0; i < TTY_COUNT; i++) {
		text_tty[i].buf = calloc(WIDTH * HEIGHT, sizeof(char));
		text_tty[i].x = 0;
		text_tty[i].y = 0;
	}

	text_current = &text_tty[0];

	text_cursor = 1;
	task_start(task_cursor, 512);
}

void text_redraw(void)
{
	for (unsigned int j = 0; j < HEIGHT; j++) {
		unsigned int i = WIDTH;
		//for (; i > 0 && text_current->buf[j * WIDTH + i - 1] == '\0'; i--);
		for (; i > 0; i--) {
			int c = text_current->buf[j * WIDTH + i - 1];
			dsp_putchar(c, i - 1, j);
		}
	}
}

void text_switch(unsigned int i)
{
	if (i >= TTY_COUNT || text_current == text_tty + i)
		return;

	text_current = &text_tty[i];
	text_redraw();
}

void text_clear(void)
{

	for (unsigned int j = 0; j < HEIGHT; j++) {
		for (unsigned int i = 0; i < WIDTH; i++) {
			//int c = text_current->buf[j * WIDTH + 1];
			//if (c != 0) {
				text_current->buf[j * WIDTH + i] = 0;
				dsp_putchar(' ', i, j);
			//}
		}
	}
	text_current->x = 0;
	text_current->y = 0;
}

void text_putchar(int c)
{
	uint8_t x = text_current->x;
	uint8_t y = text_current->y;

	switch (c) {
	case '\n':
		y++;
		// fall_through
	case '\r':
		x = 0;
		break;
	case '\t':
		for (unsigned int i = 0; i < 4; i++)
			dsp_putchar(' ', x++, y);
		break;
	case '\b':
		x--;
		dsp_putchar(' ', x, y);
		break;
	default:
		dsp_putchar(c, x, y);
		text_current->buf[x + y * WIDTH] = c;
		x++;
		break;
	}

	if (x >= WIDTH) {
		x = 0;
		y++;
	}

	if (y >= HEIGHT) {
		// clear
		for (int i = 1; i < HEIGHT; i++) {
			memcpy(text_current->buf + (i - 1) * WIDTH,
				text_current->buf + i * WIDTH, WIDTH);
		}
		x = 0;
		y = HEIGHT - 1;
		for (int i = 0; i < WIDTH; i++)
			text_current->buf[i + y * WIDTH] = 0;

		for (int j = HEIGHT - 1; j >= 0; j--) {
			int i = WIDTH - 1;
			if (j > 0) {
				for (; i >= 0; i--) {
					int p = text_current->buf[(j - 1) * WIDTH + i];
					int c = text_current->buf[j * WIDTH + i];
					if (p != '\0' || c != '\0')
						break;
				}
			}
			for (; i >= 0; i--) {
				int c = text_current->buf[j * WIDTH + i];
				dsp_putchar(c, i, j);
			}
		}
	}

	text_current->x = x;
	text_current->y = y;
}

void text_puts(const char *s)
{
	unsigned int i = 0;
	while (s[i])
		text_putchar(s[i++]);
}

void text_setpos(uint8_t x, uint8_t y)
{
	if (x >= WIDTH)
		x = WIDTH - 1;
	if (y >= HEIGHT)
		y = HEIGHT - 1;
	text_current->x = x;
	text_current->y = y;
}

void text_relpos(int8_t x, int8_t y)
{
	if ((int)(text_current->x + x) < 0)
		text_current->x = 0;
	else
		text_current->x += x;
	if ((int)(text_current->y + y) < 0)
		text_current->y = 0;
	else
		text_current->y += y;
}
