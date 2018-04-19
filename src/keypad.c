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

#define BTN_SLEEP GPIO_PORT(C, 11)

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

#define K_2ND  0x000000FF
#define K_HOLD 0x000001FF
#define K_INS  0x000002FF

static const char keypad_map[ROWS * COLS * 4] = {
	"\x7F\0\0\0" "\xFF\0\0\0" "\xFF\x02\0\0" "\x19\0\0\0" "\x18\0\0\0"
	"sin\0"      "cos\0"      "tan\0"    "\x1B\0\0\0" "\x1A\0\0\0"
	"7\0\0\0"    "8\0\0\0"    "9\0\0\0"  "(\0\0\0"    ")\0\0\0"
	"4\0\0\0"    "5\0\0\0"    "6\0\0\0"  "/\0\0\0"    "*\0\0\0"
	"1\0\0\0"    "2\0\0\0"    "3\0\0\0"  "-\0\0\0"    "+\0\0\0"
	".\0\0\0"    "0\0\0\0"    "=\0\0\0"  "\b\0\0\0"   "\n\0\0\0"
};

static const char keypad_map_2nd[ROWS * COLS * 4] = {
	"\x7F\0\0\0" "pi\0\0"  "X\0\0\0" "Y\0\0\0"  "Z\0\0\0"
	"A\0\0\0"    "B\0\0\0" "C\0\0\0" "D\0\0\0"  "E\0\0\0"
	"F\0\0\0"    "G\0\0\0" "H\0\0\0" "I\0\0\0"  "J\0\0\0"
	"K\0\0\0"    "L\0\0\0" "M\0\0\0" "N\0\0\0"  "O\0\0\0"
	"P\0\0\0"    "Q\0\0\0" "R\0\0\0" "S\0\0\0"  "T\0\0\0"
	"U\0\0\0"    "V\0\0\0" "W\0\0\0" "\b\0\0\0" "\xFF\x01\0\0"
};

#define KEY(r, c, i) map[r * COLS * 4 + c * 4 + i]
#define KEYCODE(r, c) *((uint32_t *)(map + (r * COLS * 4 + c * 4)))

#define BUFFER_SIZE 8
static char keypad_buffer[BUFFER_SIZE];
static int keypad_buffer_pos = -1;

int keypad_insert = 0;

void keypad_task(void)
{
	unsigned int col = 0;
	unsigned char use2nd = 0;
	unsigned char hold = 0;
	while (1) {
		gpio_dout(keypad_cols[col].port, keypad_cols[col].pin, 1);
		delay(10);
		for (unsigned int row = 0; row < ROWS; row++) {
			if (gpio_din(keypad_rows[row].port, keypad_rows[row].pin)) {
				char *map = (use2nd == 0) ? keypad_map : keypad_map_2nd;
				if (KEYCODE(row, col) == K_2ND) {
					use2nd = 1;
				} else if (KEYCODE(row, col) == K_HOLD) {
					if (use2nd != 0) {
						if ((hold ^= 1) == 0)
							use2nd = 0;
					}
				} else if (KEYCODE(row, col) == K_INS) {
					keypad_insert ^= 1;
				} else if (keypad_buffer_pos < BUFFER_SIZE) {
					if (use2nd != 0 && hold == 0)
						use2nd = 0;
					for (unsigned int i = 0; KEY(row, col, i) != '\0'; i++)
						keypad_buffer[++keypad_buffer_pos] = KEY(row, col, i);
				}
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

	gpio_mode(BTN_SLEEP, OUTPUT);
	gpio_dout(BTN_SLEEP, 0);
	gpio_mode(BTN_SLEEP, INPUT);
	gpio_pupd(BTN_SLEEP, PULLDOWN);

	//SYSCFG->EXTICR[2] |= 0x200; // C10
	EXTI->RTSR1 |= (1 << 11);
	EXTI->EMR1 |= (1 << 11); // Allow *10
	EXTI->IMR1 |= (1 << 11);

	// enable IRQ in NVIC
	((uint32_t *)0xE000E100)[1] |= (1 << 8);
}

void keypad_start(void)
{
	task_start(keypad_task, 1024);
}

int keypad_get(void)
{
	if (keypad_buffer_pos < 0)
		return 0;

	int key = keypad_buffer[0];
	for (int i = 0; i < BUFFER_SIZE - 1; i++)
		keypad_buffer[i] = keypad_buffer[i + 1];
	keypad_buffer_pos--;
	return key;
}

uint32_t sleep_pending = 0;
void EXTI15_10_IRQHandler(void)
{
	uint32_t PR1 = EXTI->PR1;

	if (gpio_din(BTN_SLEEP)) {
		while (gpio_din(BTN_SLEEP));
		if (sleep_pending != 0) {
			sleep_pending = 0;
			extern void wakeup(void);
			wakeup();
		} else {
			sleep_pending |= 1;
		}
		return;
	}

	EXTI->PR1 |= PR1;
}

