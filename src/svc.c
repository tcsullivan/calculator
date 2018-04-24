/**
 * @file svc.c
 * An unused handler for SVC calls
 * TODO: use SVC calls, possibly allowing for switch to unprivileged mode?
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

#include <stdint.h>
#include <gpio.h>
#include <clock.h>
//#include <lcd.h>

void SVC_Handler(void) {
	uint32_t *stack;
	uint8_t index;

	asm("\
		mrs r0, psp; \
		mov %0, r0; \
		ldr r0, [r0, #24]; \
		ldr %1, [r0, #-2]; \
	" : "=r" (stack), "=r" (index));

	gpio_mode(GPIOA, 6, OUTPUT);

	switch (index) {
	case 1:
		gpio_dout(GPIOA, 6, 1);
		for (int i = 0; i < 100000; i++)
			asm("");
		gpio_dout(GPIOA, 6, 0);
		break;
	case 2:
	//	lcd_put((char *)stack[0]);
		break;
	default:
		break;
	}
}

