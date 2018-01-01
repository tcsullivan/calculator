#include <stm32l476xx.h>
#include <clock.h>
#include <heap.h>
#include <task.h>

/**
 * Accomplishments:
 *   - GPIO in/out
 *   - got to 80MHz clock
 *   - basic heap
 *   - multitask, can exit
 */

void pulse(uint8_t byte);
void kmain(void);

int main(void)
{
	asm("cpsid i");

	// prepare flash latency for 40MHz operation
	FLASH->ACR &= ~(FLASH_ACR_LATENCY);
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS;

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	// A clk enable
	GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6);	// A5 -> output, A0 input
	GPIOA->MODER |= GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0;
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD6);
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD5_0 | GPIO_PUPDR_PUPD6_0; // pulldown for button (1)
	//if (GPIOA->IDR & 0x01)

	clock_init();
	task_init(kmain);

	while (1);
}

void task(void);
void kmain(void)
{
	asm("cpsie i");

	task_start(task, 1024);
	while (1) {
		GPIOA->BSRR |= 1 << 6;
		delay(500);
		GPIOA->BRR |= 1 << 6;
		delay(500);
	}
}

void task(void)
{
	while (1) {
		GPIOA->BSRR |= 1 << 5;
		delay(200);
		GPIOA->BRR |= 1 << 5;
		delay(200);
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
