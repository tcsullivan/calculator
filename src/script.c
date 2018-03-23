#include <script.h>

#include <builtins.h>
#include <clock.h>
#include <display.h>
#include <display_draw.h>
#include <heap.h>
#include <random.h>
#include <serial.h>
#include <stdlib.h>
#include <keypad.h>

#include <string.h>

#define igetarg_integer(it, n) ((int)igetarg(it, n)->value.f)

int script_puts(instance *it);
int script_gets(instance *it);
int script_delay(instance *it);
int script_rect(instance *it);
int script_ppos(instance *it);
int script_line(instance *it);
int script_color(instance *it);
int script_rand(instance *it);
int script_getkey(instance *it);
int script_pixel(instance *it);

void script_loadlib(instance *it)
{
	inew_cfunc(it, "print", script_puts);
	inew_cfunc(it, "gets", script_gets);
	inew_cfunc(it, "delay", script_delay);
	inew_cfunc(it, "rect", script_rect);
	inew_cfunc(it, "ppos", script_ppos);
	inew_cfunc(it, "line", script_line);
	inew_cfunc(it, "color", script_color);
	inew_cfunc(it, "rand", script_rand);
	inew_cfunc(it, "getkey", script_getkey);
	inew_cfunc(it, "pixel", script_pixel);
}

int script_puts(instance *it)
{
	variable *v = igetarg(it, 0);
	if (v->type == NUMBER) {
		char buf[33];
		snprintf(buf, 33, "%f", v->value.f);
		dsp_puts(buf);
	} else if (v->type == STRING) {
		dsp_puts((const char *)v->value.p);
	}
	return 0;
}

int script_gets(instance *it)
{
	char *s = malloc(64);
	char c[2] = {0, 0};
	int index = 0;

	do {
		c[0] = serial_get();
		s[index] = c[0];
		if (c[0] == '\b' || c[0] == 127) {
			index--;
			if (index > -1) {
				dsp_puts("\b");
				index--;
			}
		} else if (c[0] != '\r') {
			dsp_puts(c);
		}
	} while (s[index] != '\r' && index++ < 63);
	s[index] = '\0';

	variable *r = make_vars(0, s);
	ipush(it, (uint32_t)r);
	free(s);
	return 0;
}

int script_delay(instance *it)
{
	int ms = (int)igetarg(it, 0)->value.f;
	delay(ms);
	return 0;
}

int script_rect(instance *it)
{
	dsp_rect(igetarg_integer(it, 0), igetarg_integer(it, 1),
		igetarg_integer(it, 2), igetarg_integer(it, 3),
		igetarg_integer(it, 4));
	return 0;
}

int script_line(instance *it)
{
	int x = igetarg_integer(it, 0);
	int y = igetarg_integer(it, 1);
	int i = igetarg_integer(it, 2);
	int j = igetarg_integer(it, 3);
	int c = igetarg_integer(it, 4);
	dsp_line(x, y, i, j, c);
	return 0;
}

int script_ppos(instance *it)
{
	dsp_cpos(0, 0);
	dsp_coff(igetarg_integer(it, 0), igetarg_integer(it, 1));
	return 0;
}

int script_color(instance *it)
{
	uint16_t c = dsp_color(igetarg_integer(it, 0), igetarg_integer(it, 1),
		igetarg_integer(it, 2));
	variable *v = make_varf(0, (float)c);
	ipush(it, (uint32_t)v);
	return 0;
}

int script_rand(instance *it)
{
	unsigned int mod = igetarg_integer(it, 0);
	unsigned int val = random_get();
	variable *v = make_varf(0, (float)(mod % val));
	ipush(it, (uint32_t)v);
	return 0;
}

int script_getkey(instance *it)
{
	variable *v = make_varf(0, (float)keypad_get());
	ipush(it, (uint32_t)v);
	return 0;
}

int script_pixel(instance *it)
{
	dsp_pixel(igetarg_integer(it, 0), igetarg_integer(it, 1),
		igetarg_integer(it, 2));
	return 0;
}

