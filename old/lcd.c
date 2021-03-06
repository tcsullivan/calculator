/**
 * @file lcd.c
 * A basic library for writing a 16x2 text LCD.
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

#include <lcd.h>
#include <clock.h>
#include <gpio.h>
#include <string.h>

//#define USE_DELAY

#define LCD_D0 GPIO_PORT(A, 0)
#define LCD_D1 GPIO_PORT(A, 1)
#define LCD_D2 GPIO_PORT(A, 4)
#define LCD_D3 GPIO_PORT(B, 0)
#define LCD_D4 GPIO_PORT(C, 1)
#define LCD_D5 GPIO_PORT(C, 0)
#define LCD_D6 GPIO_PORT(C, 2)
#define LCD_D7 GPIO_PORT(C, 3)
#define LCD_E  GPIO_PORT(C, 12)
#define LCD_RS GPIO_PORT(C, 10)

#define lcd_data() gpio_dout(LCD_RS, 1)

void lcd_pulse(void)
{
	gpio_dout(LCD_E, 1);
#ifdef USE_DELAY
	delay(1);
#else
	for (uint16_t i = 0; i < 10000; i++)
		asm("");
#endif // USE_DELAY
	gpio_dout(LCD_E, 0);
}

void lcd_byte(uint8_t byte)
{
	gpio_dout(LCD_D0, byte & 0x01);
	gpio_dout(LCD_D1, byte & 0x02);
	gpio_dout(LCD_D2, byte & 0x04);
	gpio_dout(LCD_D3, byte & 0x08);
	gpio_dout(LCD_D4, byte & 0x10);
	gpio_dout(LCD_D5, byte & 0x20);
	gpio_dout(LCD_D6, byte & 0x40);
	gpio_dout(LCD_D7, byte & 0x80);
}

void lcd_cmd(uint8_t cmd)
{
	gpio_dout(LCD_RS, 0);
	lcd_byte(cmd);
	lcd_pulse();
}

void lcd_putchar(int c)
{
	lcd_data();
	lcd_byte((uint8_t)c);
	lcd_pulse();
}

static int lcd_index = 0;
void lcd_puts(const char *s)
{
	lcd_cmd(0x06);
	while (*s) {
		lcd_putchar(*s++);
		if (++lcd_index == 0x10) {
			lcd_cmd(0x80 | 0x40);
		} else if (lcd_index == 0x20) {
			lcd_cmd(0x80);
			lcd_index = 0;
		}
	}
}

extern char *itoa(int n, int base);
void lcd_puti(int i)
{
	lcd_puts(itoa(i, 10));
}

void lcd_puth(int h)
{
	lcd_puts(itoa(h, 16));
}

void lcd_putb(uint8_t b)
{
	lcd_puts(itoa(b, 2));
}

void lcd_clear(void)
{
	lcd_cmd(0x01);
	delay(2);
	lcd_index = 0;
}

void lcd_init(void)
{
	gpio_mode(LCD_D0, OUTPUT);
	gpio_mode(LCD_D1, OUTPUT);
	gpio_mode(LCD_D2, OUTPUT);
	gpio_mode(LCD_D3, OUTPUT);
	gpio_mode(LCD_D4, OUTPUT);
	gpio_mode(LCD_D5, OUTPUT);
	gpio_mode(LCD_D6, OUTPUT);
	gpio_mode(LCD_D7, OUTPUT);
	gpio_mode(LCD_E,  OUTPUT);
	gpio_mode(LCD_RS, OUTPUT);
	gpio_dout(LCD_D0, 0);
	gpio_dout(LCD_D1, 0);
	gpio_dout(LCD_D2, 0);
	gpio_dout(LCD_D3, 0);
	gpio_dout(LCD_D4, 0);
	gpio_dout(LCD_D5, 0);
	gpio_dout(LCD_D6, 0);
	gpio_dout(LCD_D7, 0);
	gpio_dout(LCD_E,  0);
	gpio_dout(LCD_RS, 0);

	lcd_cmd(0x38);
	lcd_cmd(0x10);
	lcd_cmd(0x0D);
	delay(5);
	lcd_clear();
}

/**
 * Task code
 */

volatile int bufpos = 0;
volatile char buf[32];
volatile uint8_t using = 0;

void lcd_clearbuf(void)
{
	bufpos = 0;
	for (int i = 0; i < 32; i++)
		buf[i] = 0;
}

void lcd_put(const char *s)
{
	int len = strlen(s);
	int i;

	using = 1;
	for (i = 0; i < len; bufpos++, i++) {
		if (bufpos > 31)
			bufpos = 0;
		buf[bufpos] = s[i];
	}
	using = 0;
}

void lcd_handler(void)
{
	lcd_init();
	lcd_clearbuf();

	while (1) {
		if (!using && buf[0] != '\0') {
			lcd_puts(buf);
			lcd_clearbuf();
		}
		delay(100);
	}
}
