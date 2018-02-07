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

int script_puts(interpreter *it)
{
	char *s = igetarg_string(it, 0);
	asm("mov r0, %0; svc 2" :: "r" (s));
	return 0;
}

void task_interpreter(void)
{
	interpreter it;
	iinit(&it);
	inew_cfunc(&it, "print", script_puts);

	while (1);
}

void kmain(void)
{
	asm("cpsie i");

	task_start(lcd_handler, 128);
	delay(200);
	task_start(task_interpreter, 2048);

	//char *s = initrd_getfile("test.txt");

	while (1) {
		gpio_dout(GPIOA, 5, 1);
		delay(500);
		gpio_dout(GPIOA, 5, 0);
		delay(500);
	}
}

