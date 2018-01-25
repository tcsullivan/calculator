#include <stm32l476xx.h>
#include <clock.h>
#include <heap.h>
#include <task.h>
#include <gpio.h>
#include <lcd.h>
#include <initrd.h>
#include <serial.h>
#include <parser.h>

/**
 * Accomplishments:
 *   - GPIO in/out
 *   - got to 80MHz clock
 *   - basic heap
 *   - multitask, can exit
 *   - gpio lib
 *   - lcd support
 *   - initrd support
 *   - lua? - no, something better, smaller
 *   - serial IO
 */

void pulse(uint8_t byte);
void kmain(void);

int main(void)
{
	asm("cpsid i");

	// prepare flash latency for 40MHz operation
	FLASH->ACR &= ~(FLASH_ACR_LATENCY);
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS;

	//MPU->CTRL |= MPU_CTRL_ENABLE_Msk | MPU_CTRL_PRIVDEFENA_Msk;
	clock_init();
	gpio_init();

	gpio_mode(GPIOA, 5, OUTPUT);

	serial_init();

	// enable FPU
	SCB->CPACR |= (0xF << 20);

	task_init(kmain);

	while (1);
}

void script_puts(stack_t *stack)
{
	asm("mov r0, %0; svc 2" :: "r" (stack[0]));
}

void task_interpreter(void)
{
	interpreter interp;
	interpreter_init(&interp);
	interpreter_define_cfunc(&interp, "print", script_puts);

	char buf[32];
	int index;
	while (1) {
		index = 0;
		do {
			buf[index] = serial_get();
		} while (buf[index++] != '\r' && index < 32);
		buf[index - 1] = '\0';
	
		interpreter_doline(&interp, buf);
	}
}

void task_suck(void)
{
	float i = 1;

	while (1) {
		i += 0.123;
		lcd_puti((int)(i * 1000));
		delay(2000);
	}
}

#include <stdio.h>
void kmain(void)
{
	asm("cpsie i");

	task_start(lcd_handler, 128);
	delay(200);
	task_start(task_suck, 1024);
	//task_start(task_interpreter, 1024);

	//char *s = initrd_getfile("test.txt");
	// svc puts
	//asm("mov r0, %0; svc 2" :: "r" (s));

	//extern void lua_start(void);
	//lua_start();

	while (1) {
		gpio_dout(GPIOA, 5, 1);
		delay(500);
		gpio_dout(GPIOA, 5, 0);
		delay(500);
	}
}

