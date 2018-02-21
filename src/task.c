#include <task.h>
#include <heap.h>
#include <stm32l476xx.h>

typedef struct {
	void *next;
	uint32_t *stack;
	uint32_t *sp;
} task_t;

static task_t *current;
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
	t->sp = (void *)t->stack + stackSize - 64;//16;
	t->sp[13] = (uint32_t)task_exit;
	t->sp[14] = (uint32_t)code;
	t->sp[15] = 0x01000000;
	return t;
}

void task_init(void (*init)(void))
{
	current = task_create(init, 4096);
	current->next = current;
	// bit 0 - priv, bit 1 - psp/msp
	asm("\
		msr psp, %0; \
		mrs r0, control; \
		orr r0, r0, #2; \
		msr control, r0; \
		isb; \
	" :: "r" (current->sp));

	task_disable = 0;
	init();
	// you dirty dirty dog
	/*asm("\
		cpsie i; \
		mov pc, %0; \
	" :: "r" (init + 4));*/
}

void task_start(void (*task)(void), uint16_t stackSize)
{
	task_hold(1);
	task_t *t = task_create(task, stackSize);
	task_t *next = (task_t *)current->next;
	current->next = t;
	t->next = next;
	task_hold(0);
}

__attribute__ ((naked))
void PendSV_Handler(void) 
{
	if (task_disable != 0)
		asm("bx lr");

	// save state
	asm("\
		cpsid i; \
		isb; \
		dsb; \
		mrs r0, psp; \
		stmdb r0!, {r4-r11}; \
		mov %0, r0; \
	" : "=r" (current->sp));

	current = current->next;

	// restore
	asm("\
		mov r0, %0; \
		ldmia r0!, {r4-r11}; \
		msr psp, r0; \
		isb; \
		dsb; \
	" :: "r" (current->sp));

	// end
	asm("\
		cpsie i; \
		bx lr; \
	");
}

