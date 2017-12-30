#include <stm32l476xx.h>
#include <clock.h>

/**
 * Accomplishments:
 *   - GPIO in/out
 *   - got to 80MHz clock
 */

void pulse(uint8_t byte);

int main(void)
{
	// prepare flash latency for 40MHz operation
	FLASH->ACR &= ~(FLASH_ACR_LATENCY);
	FLASH->ACR |= FLASH_ACR_LATENCY_2WS;

	clock_init();

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	// A clk enable
	GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6);	// A5 -> output, A0 input
	GPIOA->MODER |= GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0;
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD6);
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD5_0 | GPIO_PUPDR_PUPD6_0; // pulldown for button (1)
	//if (GPIOA->IDR & 0x01)

	while (1) {
		delay(500);
		GPIOA->BSRR |= 1 << 5;
		GPIOA->BRR |= 1 << 6;
		delay(500);
		GPIOA->BSRR |= 1 << 6;
		GPIOA->BRR |= 1 << 5;
	}
}

void _exit(int code)
{ for (;;); }

void pulse(uint8_t byte)
{
	int8_t i = 7;
	do {
		GPIOA->BSRR |= 1 << 5;
		delay((byte & (1 << i)) ? 400 : 100);
		GPIOA->BRR |= 1 << 5;
		delay((byte & (1 << i)) ? 100 : 400);
	} while (--i >= 0);
}
