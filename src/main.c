#include <stm32l476xx.h>
#include <clock.h>
#include <heap.h>
#include <task.h>
#include <gpio.h>
#include <lcd.h>
#include <display.h>
#include <display_draw.h>
#include <initrd.h>
#include <parser.h>
#include <serial.h>
#include <string.h>
#include <script.h>
#include <random.h>
#include <keypad.h>

extern uint8_t _ebss;
extern char *itoa(int, char *, int);

void kmain(void);
void task_interpreter(void);

int main(void)
{
	asm("cpsid i");
	// disable cached writes for precise debug info
	//*((uint32_t *)0xE000E008) |= 2;

	// prepare flash latency for 80MHz operation
	FLASH->ACR &= ~(FLASH_ACR_LATENCY);
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS;

	clock_init();
	heap_init(&_ebss);
	gpio_init();
	keypad_init();
	serial_init();
	random_init();

	//extern void keypad_init(void);
	//keypad_init();

	gpio_mode(GPIOA, 5, OUTPUT);

	// enable FPU
	SCB->CPACR |= (0xF << 20);
	// enable MPU
	//MPU->CTRL |= MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_ENABLE_Msk;

	task_init(kmain);
	while (1);
}

void kmain(void)
{
	dsp_init();
	dsp_rect(0, 0, LCD_WIDTH, LCD_HEIGHT, dsp_color(0, 0, 0));
	dsp_cursoron();
	task_start(task_interpreter, 4096);

	while (1) {
		gpio_dout(GPIOA, 5,
			(keypad_isdown(K0)));
		delay(10);
		/*gpio_dout(GPIOA, 5, 1);
		delay(250);
		gpio_dout(GPIOA, 5, 0);
		delay(250);*/
	}
}

void task_interpreter(void)
{
	interpreter it;
	iinit(&it);
	script_loadlib(&it);

	char *s = initrd_getfile("init");
	if (s == 0) {
		dsp_puts("init not found");
		goto end;
	}

	char *linebuf = (char *)malloc(120);
	uint32_t i = 0, prev = 0, lc;
	uint32_t size = initrd_getfilesize("init");
	int ret = 0;
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

// for interactive use
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

