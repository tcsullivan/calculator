#include <stdint.h>
#include <gpio.h>
#include <clock.h>
#include <lcd.h>

void SVC_Handler(void) {
	uint32_t *stack;
	uint8_t index;

	asm("\
		mrs r0, psp; \
		mov %0, r0; \
		ldr r0, [r0, #24]; \
		ldr %1, [r0, #-2]; \
	" : "=r" (stack), "=r" (index));

	gpio_mode(GPIOA, 6, OUTPUT);

	switch (index) {
	case 1:
		gpio_dout(GPIOA, 6, 1);
		for (int i = 0; i < 100000; i++)
			asm("");
		gpio_dout(GPIOA, 6, 0);
		break;
	case 2:
		lcd_put((char *)stack[0]);
		break;
	default:
		break;
	}
}

