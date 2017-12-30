#include "stm32l476xx.h"

#define STK_CTRL	*((uint32_t *)0xE000E010)
#define STK_LOAD	*((uint32_t *)0xE000E014)
#define STK_VAL		*((uint32_t *)0xE000E018)
#define STK_CALIB	*((uint32_t *)0xE000E01C)

extern void delay(uint32_t count);

/**
 * Accomplishments:
 *   - GPIO in/out
 *   - got to 40MHz clock
 */

int main(void)
{
	// turn on HSI
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY);

	// get PLLR to 80MHz (max)
	// VCO = C * (N/M) -> 16 * (10/1) = 160
	// SCLK = VCO / R = 160 / 4 = 40 MHz
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC);	// source HSI, 16MHz
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLM);
	RCC->PLLCFGR |= 10 << RCC_PLLCFGR_PLLN_Pos;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLR);	// /4
	RCC->PLLCFGR |= 1 << RCC_PLLCFGR_PLLR_Pos;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;		// PLLR on

	// start PLL
	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

	// set system clock to PLL
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);

	// SysTick setup? Assume 4MHz reset clock
	STK_LOAD = 40000;
	STK_CTRL |= 0x07; // AHB, inten, enable

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	// A clk enable
	GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE0);	// A5 -> output, A0 input
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD0);
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD5_0 | GPIO_PUPDR_PUPD0_1; // pd for button

	while (1) {
		delay(500);
		//if (GPIOA->IDR & 0x01)
			GPIOA->BSRR |= 1 << 5;
		delay(500);
		//else
			GPIOA->BRR |= 1 << 5;
	}
}

void _exit(int code)
{ for (;;); }
