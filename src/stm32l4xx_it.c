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
	//extern task_t *current;
	serial_puts(s);
	//char buf[200];
	//snprintf(buf, 200, "xPSR: %x\r\nPC: %x\r\nLR: %x\r\n", current->sp[0],
	//	current->sp[1], current->sp[2]);
	//serial_puts(buf);
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

