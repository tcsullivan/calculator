#include <stm32l476xx.h>
#include <serial.h>
#include <stdlib.h>
#include <task.h>

void serial_puts(const char *s)
{
	int i = 0;
	while (s[i] != 0)
		serial_put(s[i++]);
}

void perror(const char *s)
{
	extern uint32_t heap_used;
	serial_puts(s);

	static char buf[200];
	uint32_t *psp;
	asm("mrs %0, psp" : "=r"(psp));

	snprintf(buf, 200, "\r\nPC: %x\r\nLR: %x\r\nmemused: %db\r\n",
		psp[6], psp[5], heap_used);
	serial_puts(buf);
}

__attribute__ ((naked))
void NMI_Handler(void) {}

__attribute__ ((naked))
void HardFault_Handler(void)
{
	GPIOA->BSRR |= (1 << 5);
	perror("Hard Fault!");
	while (1);
}

__attribute__ ((naked))
void MemManage_Handler(void)
{
	GPIOA->BSRR |= (1 << 5);
	perror("MemManage Fault!");
	while (1);
}

__attribute__ ((naked))
void BusFault_Handler(void)
{
	GPIOA->BSRR |= (1 << 5);
	perror("Bus Fault!");
	while (1);
}

__attribute__ ((naked))
void UsageFault_Handler(void)
{
	GPIOA->BSRR |= (1 << 5);
	perror("Usage Fault!");
	while (1);
}

__attribute__ ((naked))
void DebugMon_Handler(void) {}

