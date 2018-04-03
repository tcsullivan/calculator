#include <stm32l476xx.h>
#include <gpio.h>
#include <clock.h>

#define READ  0x03
#define WRITE 0x02
#define WREN  0x06
#define WRDS  0x04

#define SCK GPIO_PORT(C, 14)
#define SI  GPIO_PORT(C, 3)
#define SO  GPIO_PORT(C, 2)
#define CS  GPIO_PORT(C, 15)

void flash_out(uint8_t);
uint8_t flash_in(void);

void flash_init(void)
{
	gpio_mode(SCK, OUTPUT);
	gpio_mode(SI, OUTPUT);
	gpio_mode(CS, OUTPUT);
	gpio_mode(SO, OUTPUT);
	gpio_dout(SO, 0);
	gpio_mode(SO, INPUT);
	gpio_dout(CS, 1);
	gpio_dout(SCK, 0);
	gpio_dout(SI, 0);

	//RCC->AHB3ENR |= RCC_AHB3ENR_QSPIEN;

	//// 10MHz operation, per datasheet
	//QUADSPI->CR &= ~(0xFF << QUADSPI_CR_PRESCALER_Pos);
	//QUADSPI->CR |= 7 << QUADSPI_CR_PRESCALER_Pos;

	//// pick FSEL! 0=1, 1=2

	//// FSIZE = 16, 2^17 bits = 1Mb
	//QUADSPI->DCR = (16 << QUADSPI_DCR_FSIZE_Pos);

	//// Memmap mode, single-spi
	//QUADSPI->CCR = (3 << QUADSPI_CCR_FMODE_Pos) | (1 << QUADSPI_CCR_DMODE_Pos)
	//	| (2 << QUADSPI_CCR_ADSIZE_Pos) | (1 << QUADSPI_CCR_ADMODE_Pos)
	//	| (1 << QUADSPI_CCR_IMODE_Pos);
	//// TODO CCR also takes instruction byte
	//QUADSPI->CCR |= (READ << QUADSPI_CCR_INSTRUCTION_Pos);

	//QUADSPI->CR |= QUADSPI_CR_EN;
}

void flash_out(uint8_t byte)
{
	for (uint8_t i = 0; i < 8; i++) {
		gpio_dout(SI, (byte & (1 << (7 - i))));
		gpio_dout(SCK, 1);
		delay(1);
		gpio_dout(SCK, 0);
	}
}

void flash_addr(uint32_t addr)
{
	for (uint8_t i = 0; i < 24; i++) {
		gpio_dout(SI, (addr & (1 << (23 - i))));
		gpio_dout(SCK, 1);
		delay(1);
		gpio_dout(SCK, 0);
	}
}

uint8_t flash_in(void)
{
	uint8_t byte = 0;
	for (uint8_t i = 0; i < 8; i++) {
		gpio_dout(SCK, 1);
		delay(1);
		gpio_dout(SCK, 0);
		if (gpio_din(SO))
			byte |= (1 << (7 - i));
	}
	return byte;
}

void flash_read(char *buf, uint32_t addr, unsigned int count)
{
	if (buf == 0)
		return;
	gpio_dout(CS, 0);
	delay(1);
	flash_out(READ);
	flash_addr(addr);
	for (unsigned int i = 0; i < count; i++)
		buf[i] = flash_in();
	gpio_dout(CS, 1);
	delay(1);
}

void flash_write(const char *buf, uint32_t addr, unsigned int count)
{
	if (buf == 0)
		return;
	gpio_dout(CS, 0);
	delay(1);
	flash_out(WREN);
	gpio_dout(CS, 1);
	delay(100);
	gpio_dout(CS, 0);
	flash_out(WRITE);
	flash_addr(addr);
	for (unsigned int i = 0; i < count; i++)
		flash_out(buf[i]);
	gpio_dout(CS, 1);
	delay(100);
	//gpio_dout(CS, 0);
	//flash_out(WRDS);
	//gpio_dout(CS, 1);
}
