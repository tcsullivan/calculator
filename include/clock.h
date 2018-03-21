/**
 * @file clock.h
 * Basic clock utilities
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>

/**
 * Sets HCLK (system clock) to 80MHz, the maximum.
 */
extern void clock_init(void);

/**
 * Sleeps for given amount of milliseconds.
 * @param ms number of milliseconds to sleep for
 */
void delay(uint32_t ms);

#endif // CLOCK_H_
