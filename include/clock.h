#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>

/**
 * Sets HCLK (system clock) to 80MHz, the maximum.
 */
extern void clock_init(void);

/**
 * Sleeps for given milliseconds.
 */
void delay(uint32_t ms);

#endif // CLOCK_H_
