/**
 * @file task.h
 * Provides multitasking functionality
 */

#ifndef TASK_H_
#define TASK_H_

#include <stdint.h>

/**
 * Enters multitasking mode. The given function acts as the initial thread.
 * This task is given a 4kb stack.
 * @param init the initial thread to run
 */
void task_init(void (*init)(void));

/**
 * Starts a new task.
 * @param task the code to run
 * @param stackSize how many bytes of stack to give the thread
 */
void task_start(void (*task)(void), uint16_t stackSize);

void task_hold(uint8_t hold);

#endif // TASK_H_
