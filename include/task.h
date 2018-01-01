#ifndef TASK_H_
#define TASK_H_

#include <stdint.h>

void task_init(void (*init)(void));

void task_start(void (*task)(void), uint16_t stackSize);

#endif // TASK_H_
