/**
 * @file main.c
 * Entry point for operating system
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
#include <display.h>
#include <display_draw.h>
#include <flash.h>
#include <gpio.h>
#include <heap.h>
#include <initrd.h>
#include <keypad.h>
#include <lcd.h>
#include <it/parser.h>
#include <random.h>
#include <script.h>
#include <serial.h>
#include <stm32l476xx.h>
#include <string.h>
#include <task.h>

extern uint8_t __bss_end__;
extern char *itoa(int, char *, int);

void kmain(void);
void task_interpreter(void);

int main(void)
{
	asm("cpsid i");
	// disable cached writes for precise debug info
	//*((uint32_t *)0xE000E008) |= 2;

	// prepare flash latency for 80MHz operation
	FLASH->ACR &= ~(FLASH_ACR_LATENCY);
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS;

	clock_init();
	heap_init(&__bss_end__);
	random_init();
	gpio_init();
	serial_init();
	keypad_init();
	flash_init();

	gpio_mode(GPIOA, 5, OUTPUT);

	// enable FPU
	SCB->CPACR |= (0xF << 20);
	// enable MPU
	//MPU->CTRL |= MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_ENABLE_Msk;

	task_init(kmain);
	while (1);
}

void kmain(void)
{
	dsp_init();
	dsp_cursoron();
	keypad_start();

	task_start(task_interpreter, 4096);

	while (1) {
		gpio_dout(GPIOA, 5, 1);
		delay(250);
		gpio_dout(GPIOA, 5, 0);
		delay(250);
	}
}

instance *load_program(const char *name)
{
	// load file
	char *s = initrd_readfile(name);
	if (s == 0) {
		dsp_puts("can't find ");
		dsp_puts(name);
		goto fail;
	}

	instance *it = inewinstance();
	script_loadlib(it);

	// read in, parse into script code
	char *linebuf = (char *)malloc(120);
	uint32_t i = 0, prev = 0, lc;
	uint32_t size = initrd_filesize(name);
	int ret = 0;
	while (i < size) {
		for (; s[i] != '\n' && s[i] != '\0'; i++);
		lc = i - prev;
		if (lc == 0) {
			prev = ++i;
			continue;
		}
		strncpy(linebuf, s + prev, lc + 1);
		linebuf[lc] = '\0';
		ret = iaddline(it, linebuf);
		if (ret != 0)
			goto fail;
		prev = ++i;
	}
	free(linebuf);
	return it;
fail:
	while (1);
	return 0;
}

void task_interpreter(void)
{
	instance *it = load_program("init");

	// run the script
	/*int ret =*/ irun(it);
	//if (ret != 0)
		//goto end;
	idelinstance(it);

//end:
	while (1)
		delay(10);
/*fail:
	if (ret < 0) {
		dsp_puts("\nError: ");
		dsp_puts(itoa(ret, linebuf, 10));
	}
	goto end;*/
}

