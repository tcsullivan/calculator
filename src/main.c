#include <stm32l476xx.h>
#include <clock.h>
#include <heap.h>
#include <task.h>
#include <gpio.h>
#include <lcd.h>
#include <display.h>
#include <initrd.h>
#include <serial.h>
#include <parser.h>
#include <stack.h>

#include <stdlib.h>
#include <string.h>

void kmain(void);
void task_interpreter(void);

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
	//lcd_puts(s);
	asm("mov r0, %0; svc 2" :: "r" (s));
	return 0;
}

int script_delay(interpreter *it)
{
	int ms = igetarg_integer(it, 0);
	delay(ms);
	return 0;
}

void task_interpreter(void)
{
	interpreter it;
	iinit(&it);
	inew_cfunc(&it, "print", script_puts);
	inew_cfunc(&it, "delay", script_delay);

	char *s = initrd_getfile("init");
	if (s == 0)
		goto end;

	char *linebuf = (char *)malloc(120);
	uint32_t i = 0, prev = 0, lc;
	uint32_t size = initrd_getfilesize("init");
	int ret;
	while (i < size) {
		for (; s[i] != '\n' && s[i] != '\0'; i++);
		lc = i - prev;
		if (lc == 0) {
			prev = ++i;
			continue;
		}
		strncpy(linebuf, s + prev, lc + 1);
		linebuf[lc] = '\0';
		ret = idoline(&it, linebuf);
		if (ret < 0)
			break;
		prev = ++i;
	}

	if (ret < 0) {
		lcd_puts("Error: ");
		lcd_puts(itoa(ret, linebuf, 10));
	}
	free(linebuf);
	//iend(&it); // nah

end:
	while (1)
		delay(10);
}

void kmain(void)
{
	asm("cpsie i");

	dsp_init();
    dsp_set_addr(0, 0, LCD_WIDTH, LCD_HEIGHT);
    int w = LCD_WIDTH * LCD_HEIGHT;
    do {
        dsp_write_data(0);//c >> 8);
        dsp_write_data(0);//c & 0xFF);
    } while (w--);

	//task_start(lcd_handler, 128);
	//delay(200);
	//task_start(task_interpreter, 4096);

	//char *s = initrd_getfile("test.txt");

	while (1) {
		gpio_dout(GPIOA, 5, 1);
		delay(500);
		gpio_dout(GPIOA, 5, 0);
		delay(500);
	}
}

