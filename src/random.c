/**
 * @file random.c
 * Provides true random number generation functionality
 *
 * Copyright (C) 2018 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
