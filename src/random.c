#include <random.h>

#include <stm32l476xx.h>
#include <clock.h>

void random_init(void)
{
	// setup and enable RNG clock
	RCC->CCIPR &= ~(RCC_CCIPR_CLK48SEL);
	RCC->CCIPR |= RCC_CCIPR_CLK48SEL_1;
	RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
	RNG->CR |= RNG_CR_RNGEN;
}

uint32_t random_get(void)
{
	if (RNG->SR & (RNG_SR_SEIS | RNG_SR_CEIS))
		return 0;

	while (!(RNG->SR & RNG_SR_DRDY))
		delay(1);

	return RNG->DR;
}
