#include <stm32l476xx.h>
#include <gpio.h>
#include <clock.h>

#define READ  0x03
#define WRITE 0x02
#define WREN  0x06
#define WRDS  0x04

#define NSS  GPIO_PORT(B, 12)
#define SCK  GPIO_PORT(B, 13)
#define MISO GPIO_PORT(B, 14)
#define MOSI GPIO_PORT(B, 15)

void flash_xchg(char *byte, int count);

void flash_init(void)
{
	gpio_mode(NSS, ALTERNATE);
	gpio_mode(SCK, ALTERNATE);
	gpio_mode(MISO, ALTERNATE);
	gpio_mode(MOSI, ALTERNATE);
	GPIOB->AFR[1] |= 0x55550000; // alt mode SPI2

	// clock enable
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;

	SPI2->CR1 &= ~(SPI_CR1_BR_Msk);
	SPI2->CR1 |= (3 << SPI_CR1_BR_Pos);
	SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
	SPI2->CR1 |= SPI_CR1_MSTR;
	SPI2->CR2 &= ~(SPI_CR2_DS_Msk);
	SPI2->CR2 |= (7 << SPI_CR2_DS_Pos);
	SPI2->CR2 |= SPI_CR2_SSOE;
	SPI2->CR2 |= SPI_CR2_FRXTH;
	SPI2->CR1 |= SPI_CR1_SPE;

	char buf[3];
	buf[0] = READ;
	buf[1] = 0;
	buf[2] = 0;
	buf[3] = 0;
	flash_xchg(buf, 4);
}

void flash_xchg(char *byte, int count)
{
	uint32_t status = 0, dummy;
	SPI2->CR1 &= ~(SPI_CR1_SSI);
	while (SPI2->SR & SPI_SR_BSY);
	for (int i = 0; i < count; i++) {
		SPI2->DR = byte[i];
		do status = SPI2->SR;	
		while (status & SPI_SR_BSY);
		// discard rx
		while (status & SPI_SR_RXNE) {
			dummy = SPI2->DR;
			status = SPI2->SR;
		}
	}
	do status = SPI2->SR;	
	while (status & SPI_SR_BSY);

	while (1) {
		SPI2->DR = 0;
		do status = SPI2->SR;	
		while (status & SPI_SR_BSY);
		// discard rx
		while (status & SPI_SR_RXNE) {
			dummy = SPI2->DR;
			status = SPI2->SR;
		}
	}

	SPI2->CR1 |= SPI_CR1_SSI;
	(void)dummy;
}

void flash_read(char *buf, uint32_t addr, unsigned int count)
{
	(void)buf;
	(void)addr;
	(void)count;
	if (buf == 0)
		return;
}

void flash_write(const char *buf, uint32_t addr, unsigned int count)
{
	(void)buf;
	(void)addr;
	(void)count;
	if (buf == 0)
		return;
}
