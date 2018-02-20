#include <stm32l476xx.h>
#include <lcd.h>

void perror(const char *s)
{
	(void)s;//lcd_puts(s);
}

void NMI_Handler(void) {}

void HardFault_Handler(void)
{
	GPIOA->BSRR |= (1 << 5);
	perror("Hard Fault!");
	while (1);
}

void MemManage_Handler(void)
{
	GPIOA->BSRR |= (1 << 5);
	perror("MemManage Fault!");
	while (1);
}

void BusFault_Handler(void)
{
	GPIOA->BSRR |= (1 << 5);
	perror("Bus Fault!");
	while (1);
}

void UsageFault_Handler(void)
{
	GPIOA->BSRR |= (1 << 5);
	perror("Usage Fault!");
	while (1);
}

void DebugMon_Handler(void) {}

