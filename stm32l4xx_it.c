#include <stm32l476xx.h>

void NMI_Handler(void) {}

void HardFault_Handler(void)
{
	GPIOA->BSRR |= (1 << 5) | (1 << 6);
	while (1);
}

void MemManage_Handler(void)
{
	GPIOA->BSRR |= (1 << 5) | (1 << 6);
	while (1);
}

void BusFault_Handler(void)
{
	GPIOA->BSRR |= (1 << 5) | (1 << 6);
	while (1);
}

void UsageFault_Handler(void)
{
	GPIOA->BSRR |= (1 << 5) | (1 << 6);
	while (1);
}

void SVC_Handler(void) {}

void DebugMon_Handler(void) {}

