/**
 * @file task.c
 * Provides multitasking functionality
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

#include <task.h>
#include <heap.h>
#include <stm32l476xx.h>

task_t *current;
static uint8_t task_disable = 0;

void task_hold(uint8_t hold)
{
	if (hold != 0)
		task_disable++;
	else if (task_disable > 0)
		task_disable--;
}

void task_exit(void)
{
	// TODO free stack?
	// TODO remove from chain
	// hopefully current is preserved..?
	while (1); // bye
}

task_t *task_create(void (*code)(void), uint32_t stackSize)
{
	task_t *t = (task_t *)malloc(sizeof(task_t));
	t->next = 0;
	t->stack = (uint32_t *)malloc(stackSize);
	void *sp = (uint8_t *)t->stack + stackSize - 68; // excep. stack + regs
	t->sp = (uint32_t *)sp;
	for (uint8_t i = 0; i < 14; i++)
		t->sp[i] = 0;
	t->sp[8] = 0xFFFFFFFD;
	t->sp[14] = 0xFFFFFFFD;
	t->sp[15] = (uint32_t)code;
	t->sp[16] = 0x01000000;
	return t;
}

void task_init(void (*init)(void))
{
	current = task_create(init, 4096);
	current->next = current;
	task_disable = 0;

	// bit 0 - priv, bit 1 - psp/msp
	asm("\
		msr psp, %0; \
		mrs r0, control; \
		orr r0, r0, #3; \
		cpsie i; \
		msr control, r0; \
		isb; \
		bx %1; \
	" :: "r" (current->sp), "r" (init));
}

void task_start(void (*task)(void), uint16_t stackSize)
{
	task_hold(1);
	task_t *t = task_create(task, stackSize);
	t->next = current->next;
	current->next = t;
	task_hold(0);
}

__attribute__ ((naked))
void PendSV_Handler(void) 
{
	if (task_disable != 0)
		asm("bx lr");

	asm("\
		mrs r0, psp; \
		isb; \
		ldr r1, =current; \
		ldr r2, [r1]; \
		stmdb r0!, {r4-r11, r14}; \
		str r0, [r2, #8]; \
		ldr r0, [r2, #0]; \
		str r0, [r1]; \
		ldr r2, [r1]; \
		ldr r0, [r2, #8]; \
		ldmia r0!, {r4-r11, r14}; \
		msr psp, r0; \
		bx lr; \
	");

	/*// save state
	asm("\
		cpsid i; \
		isb; \
		dsb; \
		mrs r0, psp; \
		stmdb r0!, {r4-r11, lr}; \
		mov %0, r0; \
	" : "=r" (current->sp));

	current = current->next;

	// restore
	asm("\
		mov r0, %0; \
		ldmia r0!, {r4-r11, lr}; \
		msr psp, r0; \
		isb; \
		dsb; \
		cpsie i; \
		bx lr; \
	" :: "r" (current->sp));*/
}

