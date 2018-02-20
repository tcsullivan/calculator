#include <stm32l476xx.h>
#include <clock.h>
#include <heap.h>
#include <task.h>
#include <gpio.h>
#include <lcd.h>
#include <display.h>
#include <display_draw.h>
#include <initrd.h>
#include <serial.h>
#include <parser.h>
#include <stack.h>
#include <string.h>

extern char *itoa(int, char *, int);

void kmain(void);

int main(void)
{
	asm("cpsid i");

	// prepare flash latency for 40MHz operation
	FLASH->ACR &= ~(FLASH_ACR_LATENCY);
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS;

	//MPU->CTRL |= MPU_CTRL_ENABLE_Msk | MPU_CTRL_PRIVDEFENA_Msk;
	clock_init();
	extern uint8_t _ebss;
	heap_init(&_ebss);
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
	dsp_puts(s);
	//dsp_puts("\n");
	//asm("mov r0, %0; svc 2" :: "r" (s));
	return 0;
}

int script_gets(interpreter *it)
{
	char *s = malloc(64), c[2] = {0, 0};
	uint16_t index = 0;

	do {
		c[0] = serial_get();
		s[index] = c[0];
		if (c[0] != '\r')
			dsp_puts(c);
	} while (s[index] != '\r' && index++ < 23);
	s[index] = '\0';

	variable *v = igetarg(it, 0);
	v->valtype = STRING;
	v->svalue = s;
	return 0;
}

int script_delay(interpreter *it)
{
	int ms = igetarg_integer(it, 0);
	delay(ms);
	return 0;
}

int script_rect(interpreter *it)
{
	dsp_rect(igetarg_integer(it, 0), igetarg_integer(it, 1),
		igetarg_integer(it, 2), igetarg_integer(it, 3),
		igetarg_integer(it, 4));
	return 0;
}

int script_line(interpreter *it)
{
	dsp_line(igetarg_integer(it, 0), igetarg_integer(it, 1),
		igetarg_integer(it, 2), igetarg_integer(it, 3),
		igetarg_integer(it, 4));
	return 0;
}

int script_ppos(interpreter *it)
{
	dsp_cpos(0, 0);
	dsp_coff(igetarg_integer(it, 0), igetarg_integer(it, 1));
	return 0;
}

void task_interpreter(void)
{
	interpreter it;
	iinit(&it);
	inew_cfunc(&it, "print", script_puts);
	inew_cfunc(&it, "gets", script_gets);
	inew_cfunc(&it, "delay", script_delay);
	inew_cfunc(&it, "rect", script_rect);
	inew_cfunc(&it, "ppos", script_ppos);
	inew_cfunc(&it, "line", script_line);

	/*int ret = 0;
	char *linebuf = malloc(100), c[2] = {0, 0};
	while (1) {
		uint16_t index = 0;
		if (it.indent > 0)
			dsp_puts(">");
		dsp_puts("> ");
		do {
			c[0] = serial_get();
			if (c[0] >= ' ' || c[0] == '\r') {
				linebuf[index] = c[0];
				if (c[0] >= ' ')
					dsp_puts(c);
			}
		} while (linebuf[index] != '\r' && index++ < 100);
		linebuf[index] = '\0';
		dsp_puts("\n");
		ret = idoline(&it, linebuf);
		if (ret < 0)
			break;
	}*/

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
		dsp_puts("\nError: ");
		dsp_puts(itoa(ret, linebuf, 10));
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
	dsp_rect(0, 0, LCD_WIDTH, LCD_HEIGHT, dsp_color(0, 0, 0));
	dsp_cursoron();
	//dsp_puts("Hey.");
	task_start(task_interpreter, 4096);

	while (1) {
		gpio_dout(GPIOA, 5, 1);
		delay(500);
		gpio_dout(GPIOA, 5, 0);
		delay(500);
	}
}

