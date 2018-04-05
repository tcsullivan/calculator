#include <clock.h>
#include <gpio.h>
#include <heap.h>
#include <stm32l476xx.h>
#include <string.h>

#define READ  0x03
#define WRITE 0x02
#define WREN  0x06
#define WRDS  0x04

#define SCK GPIO_PORT(D, 2)
#define SI  GPIO_PORT(C, 3)
#define SO  GPIO_PORT(C, 2)
#define CS  GPIO_PORT(B, 7)

void flash_spi_xchg(char *buf, unsigned int count)
{
	gpio_dout(CS, 0);
	// for each byte
	for (unsigned int i = 0; i < count; i++) {
		for (int b = 7; b >= 0; b--) {
			gpio_dout(SI, buf[i] & (1 << b));
			gpio_dout(SCK, 1);
			if (gpio_din(SO))
				buf[i] |= 1 << b;
			else
				buf[i] &= ~(1 << b);
			gpio_dout(SCK, 0);
		}
	}
	gpio_dout(CS, 1);
}

void flash_init(void)
{
	gpio_mode(SCK, OUTPUT);
	gpio_mode(SI, OUTPUT);
	gpio_mode(CS, OUTPUT);
	gpio_mode(SO, OUTPUT);
	gpio_speed(SCK, VERYHIGH);
	gpio_speed(SI, VERYHIGH);
	gpio_speed(SO, VERYHIGH);
	gpio_speed(CS, VERYHIGH);
	gpio_dout(SO, 0);
	gpio_mode(SO, INPUT);
	gpio_dout(CS, 1);
	gpio_dout(SCK, 0);
	gpio_dout(SI, 0);
}

void flash_read(char *buf, uint32_t addr, unsigned int count)
{
	if (buf == 0)
		return;

	char *spibuf = malloc(1028);
	unsigned int c = count / 1024;
	unsigned int i = 0;
	uint32_t paddr = addr;
	for (i = 0; i < c; i++, paddr += 1024) {
		spibuf[0] = READ;
		spibuf[1] = (paddr >> 16) & 0xFF;
		spibuf[2] = (paddr >> 8) & 0xFF;
		spibuf[3] = paddr & 0xFF;
		//memcpy(spibuf + 4, buf, count);
		flash_spi_xchg(spibuf, 1028);
		memcpy(buf + i * 1024, spibuf + 4, 1024);
	}

	c = count - i * 1024;
	spibuf[0] = READ;
	spibuf[1] = (paddr >> 16) & 0xFF;
	spibuf[2] = (paddr >> 8) & 0xFF;
	spibuf[3] = paddr & 0xFF;
	flash_spi_xchg(spibuf, 4 + c);
	memcpy(buf + i * 1024, spibuf + 4, c);

	free(spibuf);
}

void flash_small_write(char *buf, uint32_t addr, unsigned int count)
{
	char wren = WREN;
	flash_spi_xchg(&wren, 1);
	delay(10);

	buf[0] = WRITE;
	buf[1] = (addr >> 16) & 0xFF;
	buf[2] = (addr >> 8) & 0xFF;
	buf[3] = addr & 0xFF;
	flash_spi_xchg(buf, count);
	delay(10);
}

void flash_write(const char *buf, uint32_t addr, unsigned int count)
{
	if (buf == 0)
		return;

	char *spibuf = malloc(260); // 4 header + 256 page size

	uint32_t startaddr = addr;
	uint16_t counter = 0;
	unsigned int offset = 0;
	for (uint32_t i = 0; i < count; i++) {
		if (i > 0 && ((addr + i) & 0xFF) == 0) {
			memcpy(spibuf + 4, buf + offset, counter);
			flash_small_write(spibuf, startaddr, 4 + counter);

			offset += counter;
			startaddr += counter;
			counter = 0;
		}
		counter++;
	}

	if (offset < count) {
		memcpy(spibuf + 4, buf + offset, count - offset);
		flash_small_write(spibuf, startaddr, 4 + count - offset);
	}

	free(spibuf);
}
