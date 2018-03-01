/**
 * @file random.h
 * Provides true random number generation functionality
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
