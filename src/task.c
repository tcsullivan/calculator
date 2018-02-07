#include <task.h>
#include <heap.h>
#include <stm32l476xx.h>

typedef struct {
	uint32_t *sp;
	uint8_t use;
	uint32_t *stack;
	void (*code)(void);
} task_t;

#define MAX_TASKS	6

static task_t tasks[MAX_TASKS];
static volatile int next_idx = 0;

static uint8_t task_enable = 0;

void task_init(void (*init)(void))
{
	for (int i = 0; i < MAX_TASKS; i++)
		tasks[i].use = 0;

	task_start(init, 1024);
	asm("\
		msr psp, %0; \
		mrs r0, control; \
		orr r0, r0, #3; \
		msr control, r0; \
		isb; \
	" :: "r" (tasks[0].sp));

	task_enable = 1;
	init();
}

extern void _exit(int);
void task_exit(void)
{
	// TODO free stack?
	asm("cpsid i"); // hope to catch next_idx
	tasks[next_idx].use = 0;
	asm("cpsie i");
	while (1); // bye
}

void task_start(void (*task)(void), uint16_t stackSize)
{
	asm("cpsid i"); // just to be safe

	for (int i = 0; i < MAX_TASKS; i++) {
		if (tasks[i].use == 0) {
			tasks[i].stack = malloc(stackSize);
			tasks[i].sp = tasks[i].stack + stackSize - 16;
			tasks[i].sp[13] = (uint32_t)task_exit;
			tasks[i].sp[14] = (uint32_t)task;
			tasks[i].sp[15] = 0x01000000;
			tasks[i].use = 1;
			tasks[i].code = task;
			asm("cpsie i");
			return;
		}
	}

	// TODO handle error
}

__attribute__ ((naked))
void PendSV_Handler(void) 
{
	if (task_enable == 0)
		asm("bx lr");

	// save state
	asm("\
		cpsid i; \
		isb; \
		dsb; \
		mrs r0, psp; \
		stmdb r0!, {r4-r11}; \
		mov %0, r0; \
	" : "=r" (tasks[next_idx].sp));

	// find next task (round-robin style)
	do {
		if (++next_idx == MAX_TASKS) {
			next_idx = 0;
			break; // task 0 better exist
		}
	} while (tasks[next_idx].use == 0);

	// restore
	asm("\
		mov r0, %0; \
		ldmia r0!, {r4-r11}; \
		msr psp, r0; \
		isb; \
		dsb; \
	" :: "r" (tasks[next_idx].sp));

	// end
	asm("\
		mov r0, #0xFFFFFFFD; \
		cpsie i; \
		bx lr; \
	");
}

