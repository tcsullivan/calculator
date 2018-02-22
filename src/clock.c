#include <clock.h>
#include <stm32l476xx.h>

#define STK_CTRL	*((uint32_t *)0xE000E010)
#define STK_LOAD	*((uint32_t *)0xE000E014)
#define STK_VAL		*((uint32_t *)0xE000E018)
#define STK_CALIB	*((uint32_t *)0xE000E01C)

// ticks since init
static uint32_t ticks = 0;

void clock_init(void)
{
	// turn on HSI (16MHz)
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY);

	// get PLLR to 80MHz (max)
	// VCO = C * (N/M) -> 16 * (10/1) = 160
	// SCLK = VCO / R = 160 / 2 = 80 MHz
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC);
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLM);
	RCC->PLLCFGR |= 10 << RCC_PLLCFGR_PLLN_Pos;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLR);	// /2
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;		// PLLR on

	// start PLL
	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

	// set system clock to PLL
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);

	// SysTick init. 80MHz / 80000 = 1kHz, ms precision
	STK_LOAD = 80000;
	STK_CTRL |= 0x07; // no div, interrupt, enable
}

void delay(uint32_t count)
{
	uint32_t target = ticks + count;
	while (ticks < target);
}

__attribute__ ((naked))
void SysTick_Handler(void)
{
	uint32_t lr;
	asm("mov %0, lr" : "=r" (lr));

	// just keep counting
	ticks++;

	if (!(ticks % 4))
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

	asm("mov lr, %0; bx lr" :: "r" (lr));
}

