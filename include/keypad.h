#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <stdint.h>

#define K0 (uint16_t)(1 << 0)
#define K1 (uint16_t)(1 << 1)
#define K2 (uint16_t)(1 << 2)
#define K3 (uint16_t)(1 << 3)
#define K4 (uint16_t)(1 << 4)
#define K5 (uint16_t)(1 << 5)
#define K6 (uint16_t)(1 << 6)
#define K7 (uint16_t)(1 << 7)
#define K8 (uint16_t)(1 << 8)
#define K9 (uint16_t)(1 << 9)
#define KS (uint16_t)(1 << 10)
#define KP (uint16_t)(1 << 11)

void keypad_init(void);

uint16_t keypad_get(void);
uint8_t keypad_isdown(uint16_t);

#endif // KEYPAD_H_
