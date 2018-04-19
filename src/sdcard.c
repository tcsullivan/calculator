/**
 * @file sdcard.c
 * Provides a basic library for accessing an SD card
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
#include <gpio.h>
#include <heap.h>
#include <sdcard.h>
#include <string.h>

#define SCK GPIO_PORT(A, 5)
#define SO  GPIO_PORT(A, 6)
#define SI  GPIO_PORT(A, 7)
#define CS  GPIO_PORT(B, 6)

typedef struct {
	uint8_t cmd;
	uint32_t arg;
	uint8_t crc;
} __attribute__ ((packed)) sdcmd_t;

typedef struct {
	uint8_t zero    :1;
	uint8_t param   :1;
	uint8_t address :1;
	uint8_t erase   :1;
	uint8_t crc     :1;
	uint8_t badcmd  :1;
	uint8_t ereset  :1;
	uint8_t idle    :1;
} __attribute__ ((packed)) r1_t;

void flash_out(uint8_t byte)
{
	for (int i = 0; i < 8; i++) {
		gpio_dout(SI, byte & (1 << (7 - i)));
		gpio_dout(SCK, 1);
		gpio_dout(SCK, 0);
	}
}

uint8_t flash_in(void)
{
	uint8_t byte = 0;
	for (int i = 0; i < 8; i++) {
		if (gpio_din(SO))
			byte |= 1 << (7 - i);
		gpio_dout(SCK, 1);
		gpio_dout(SCK, 0);
	}
	return byte;
}

void sd_delay(void)
{
	gpio_dout(SI, 1);
	uint8_t so;
	int i = 0;
	do {
		so = flash_in();
		if (i < 8)
			i++;
	} while (i < 8 || so != 0xFF);
}

uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg)
{
	sdcmd_t cmdp;
	cmdp.cmd = (cmd & 0x3F) | 0x40;
	cmdp.arg = arg;

	sd_delay();
	flash_out(cmdp.cmd);
	flash_out(cmdp.arg >> 24);
	flash_out(cmdp.arg >> 16);
	flash_out(cmdp.arg >> 8);
	flash_out(cmdp.arg);
	flash_out(cmd == 8 ? 0x87 : 0x95);

	// wait for a zero
	gpio_dout(SI, 1);
	uint8_t r1;
	do {
		r1 = flash_in();
	} while (r1 & 0x80);

	return r1;
}

void sd_init(void)
{
	gpio_mode(SCK, OUTPUT);
	gpio_mode(SI, OUTPUT);
	gpio_mode(CS, OUTPUT);
	gpio_mode(SO, OUTPUT);
	gpio_speed(SCK, VERYHIGH);
	gpio_speed(SI, VERYHIGH);
	gpio_speed(SO, VERYHIGH);
	gpio_speed(CS, VERYHIGH);
	gpio_pupd(SCK, PULLUP);
	gpio_pupd(SI, PULLUP);
	gpio_pupd(SO, PULLUP);
	gpio_pupd(CS, PULLUP);
	gpio_dout(SO, 0);
	gpio_mode(SO, INPUT);
	gpio_dout(CS, 1);
	gpio_dout(SCK, 0);
	gpio_dout(SI, 1);

	// init? cs and si are high
	delay(10);
	sd_delay();

	// pull cs low, send cmd0
	gpio_dout(CS, 0);
	uint8_t resp = sd_send_cmd(0, 0);
	sd_delay();
	gpio_dout(CS, 1);

	if (resp != 0x01)
		while (1);

	// do cmd8
	delay(10);
	gpio_dout(CS, 0);
	resp = sd_send_cmd(8, 0x1AA);
	uint8_t ocr[4];
	for (int i = 0; i < 4; i++)
		ocr[i] = flash_in();
	sd_delay();
	gpio_dout(CS, 1);
	(void)ocr;

	// initialize
	do {
		// cmd55
		gpio_dout(CS, 0);
		resp = sd_send_cmd(55, 0);
		sd_delay();
		gpio_dout(CS, 1);
		delay(10);

		// acmd41
		gpio_dout(CS, 0);
		resp = sd_send_cmd(41, 0x40000000);
		sd_delay();
		gpio_dout(CS, 1);
	} while (resp != 0x00);

	// set block length
	gpio_dout(CS, 0);
	resp = sd_send_cmd(16, 512);
	sd_delay();
	gpio_dout(CS, 1);
}

uint8_t *sd_read_block(uint8_t *buf, uint32_t lba)
{
	if (buf == 0)
		return 0;

	// send command
	gpio_dout(CS, 0);
	if (sd_send_cmd(17, lba) != 0)
		return 0;

	// wait for block
	gpio_dout(SI, 1);
	uint8_t byte;
	do byte = flash_in();
	while (byte == 0xFF);
	if (byte != 0xFE)
		return 0;

	// get block
	uint32_t i = 0;
	for (i = 0; i < 512; i++)
		buf[i] = flash_in();

	sd_delay();
	gpio_dout(CS, 1);
	return buf;
}

uint8_t *sd_read(uint8_t *buf, uint32_t lba, uint32_t count)
{
	if (buf == 0)
		return 0;

	uint8_t *block_buf = malloc(512);
	uint32_t i = 0;
	while (count > 0) {
		sd_read_block(block_buf, lba);
		memcpy(buf + i, block_buf, (count < 512) ? count : 512);
		if (count < 512)
			count = 0;
		else
			count -= 512;
		i += 512;
		lba++;
	}

	free(block_buf);
	return buf;
}

