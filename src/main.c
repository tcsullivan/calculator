#include <stm32l476xx.h>
#include <clock.h>
#include <heap.h>
#include <task.h>
#include <gpio.h>
#include <lcd.h>
#include <initrd.h>
#include <serial.h>

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

	task_init(kmain);

	while (1);
}

void serial_getter(void);
void kmain(void)
{
	asm("cpsie i");

	task_start(lcd_handler, 128);
	delay(200);

	//char *s = initrd_getfile("test.txt");
	// svc puts
	//asm("mov r0, %0; svc 2" :: "r" (s));

	task_start(serial_getter, 128);
	while (1) {
		gpio_dout(GPIOA, 5, 1);
		delay(500);
		gpio_dout(GPIOA, 5, 0);
		delay(500);
	}
}

void serial_getter(void)
{
	char buf[2] = { 0, 0 };
	while (1) {
		buf[0] = serial_get();
		asm("mov r0, %0; svc 2" :: "r" (buf));
	}
}
