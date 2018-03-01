/**
 * @file task.h
 * Provides multitasking functionality
 */

#ifndef TASK_H_
#define TASK_H_

#include <stdint.h>

typedef struct {
	void *next;
	uint32_t *stack;
	uint32_t *sp;
} task_t;

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

/**
 * Allows task switching to be disabled, for low-level actions.
 * Multiple holds can be placed, and all must be removed to continue task
 * switching.
 * @param hold non-zero for hold, zero to remove hold
 */
void task_hold(uint8_t hold);

#endif // TASK_H_
