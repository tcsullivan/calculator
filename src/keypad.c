/**
 * @file keypad.c
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

#include <clock.h>
#include <keypad.h>
#include <gpio.h>
#include <task.h>

#define ROW_0 GPIO_PORT(B, 2)
#define ROW_1 GPIO_PORT(B, 1)
#define ROW_2 GPIO_PORT(B, 15)
#define ROW_3 GPIO_PORT(B, 14)
#define ROW_4 GPIO_PORT(B, 13)
#define ROW_5 GPIO_PORT(C, 4)
#define COL_0 GPIO_PORT(B, 11)
#define COL_1 GPIO_PORT(B, 12)
#define COL_2 GPIO_PORT(A, 11)
#define COL_3 GPIO_PORT(A, 12)
#define COL_4 GPIO_PORT(C, 5)

#define ROWS 6
#define COLS 5

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} port_t;

static const port_t keypad_rows[ROWS] = {
	{ ROW_0 }, { ROW_1 }, { ROW_2 },
	{ ROW_3 }, { ROW_4 }, { ROW_5 }
};

static const port_t keypad_cols[COLS] = {
	{ COL_0 }, { COL_1 }, { COL_2 },
	{ COL_3 }, { COL_4 }
};

static const int keypad_map[ROWS][COLS] = {
	{ '&', '|', '^',  ' ',  ' ' },
	{ 'x', 'y', 'z',  '=',  ' ' },
	{ '7', '8', '9',  '(',  ')' },
	{ '4', '5', '6',  '/',  '%' },
	{ '1', '2', '3',  '*',  '-' },
	{ '.', '0', '\b', '\n', '+' }
};

#define BUFFER_SIZE 8
static char keypad_buffer = 0;//[BUFFER_SIZE];
//static int keypad_buffer_pos = -1;

void keypad_task(void)
{
	unsigned int col = 0;
	while (1) {
		gpio_dout(keypad_cols[col].port, keypad_cols[col].pin, 1);
		delay(10);
		for (unsigned int row = 0; row < ROWS; row++) {
			if (gpio_din(keypad_rows[row].port, keypad_rows[row].pin)) {
				//if (keypad_buffer_pos < BUFFER_SIZE)
					keypad_buffer/*[++keypad_buffer_pos]*/ = keypad_map[row][col];
				while (gpio_din(keypad_rows[row].port, keypad_rows[row].pin))
					delay(1);
				break;
			}
		}

		gpio_dout(keypad_cols[col].port, keypad_cols[col].pin, 0);
		col++;
		if (col == COLS)
			col = 0;
	}
}

void keypad_init(void)
{
	for (int i = 0; i < ROWS; i++) {
		GPIO_TypeDef *p = keypad_rows[i].port;
		uint16_t pin = keypad_rows[i].pin;
		gpio_mode(p, pin, OUTPUT);
		gpio_speed(p, pin, VERYHIGH);
		gpio_dout(p, pin, 0);
		gpio_mode(p, pin, INPUT);
		gpio_pupd(p, pin, PULLDOWN);
	}

	for (int i = 0; i < COLS; i++) {
		GPIO_TypeDef *p = keypad_cols[i].port;
		uint16_t pin = keypad_cols[i].pin;
		gpio_mode(p, pin, OUTPUT);
		gpio_speed(p, pin, VERYHIGH);
		gpio_dout(p, pin, 0);
	}
}

void keypad_start(void)
{
	task_start(keypad_task, 1024);
}

int keypad_get(void)
{
	//if (keypad_buffer_pos < 0)
	//	return 0;

	//int key = keypad_buffer[0];
	//for (int i = keypad_buffer_pos - 1; i > 0; i--)
	//	keypad_buffer[i - 1] = keypad_buffer[i];
	//keypad_buffer_pos--;
	//return key;
	int ret = keypad_buffer;
	keypad_buffer = 0;
	return ret;
}
