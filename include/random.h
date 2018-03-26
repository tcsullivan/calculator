/**
 * @file random.h
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

#ifndef RANDOM_H_
#define RANDOM_H_

#include <stdint.h>

/**
 * Initializes the STM's true random number generator.
 */
void random_init(void);

/**
 * Gets the next random number from the generator.
 * @return the random number
 */
uint32_t random_get(void);

#endif // RANDOM_H_
