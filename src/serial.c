#include <stm32l476xx.h>
#include <gpio.h>

void serial_init(void)
{
	gpio_mode(GPIOA, 2, ALTERNATE);
	gpio_mode(GPIOA, 3, ALTERNATE);
	GPIOA->AFR[0] &= ~(0x0000FF00);
	GPIOA->AFR[0] |= 0x00007700;
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

	// start usart device
	USART2->BRR = 80000000L / 115200L;
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void serial_put(int c)
{
	while (!(USART2->ISR & USART_ISR_TXE));
	USART2->TDR = c & 0xFF;
}

char serial_get(void)
{
	while (!(USART2->ISR & USART_ISR_RXNE));
	return USART2->RDR & 0xFF;
}

void serial_gets(char *buf)
{
	uint16_t index = 0;

	do {
		buf[index] = serial_get();
	} while (buf[index++] != '\r');

	buf[index - 1] = '\0';
	//return buf;
}
