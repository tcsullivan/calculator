#include <script.h>

#include <builtins.h>
#include <clock.h>
#include <display.h>
#include <display_draw.h>
#include <heap.h>
#include <random.h>
#include <serial.h>
#include <stack.h>
#include <keypad.h>

#include <string.h>

int script_puts(interpreter *it);
int script_gets(interpreter *it);
int script_delay(interpreter *it);
int script_rect(interpreter *it);
int script_ppos(interpreter *it);
int script_line(interpreter *it);
int script_color(interpreter *it);
int script_rand(interpreter *it);
int script_getkey(interpreter *it);
int script_pixel(interpreter *it);
int script_solve(interpreter *it);

void script_loadlib(interpreter *it)
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
	inew_cfunc(it, "solve", script_solve);
}

int script_puts(interpreter *it)
{
	const char *s = igetarg_string(it, 0);
	dsp_puts(s);
	return 0;
}

int script_gets(interpreter *it)
{
	char *s = malloc(64);
	char c[2] = {0, 0};
	uint16_t index = 0;

	do {
		c[0] = serial_get();
		s[index] = c[0];
		if (c[0] != '\r')
			dsp_puts(c);
	} while (s[index] != '\r' && index++ < 23);
	s[index] = '\0';

	variable *r = make_vars(0, s);
	iret(it, r);
	free(s);
	free(r);
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
	int x = igetarg_integer(it, 0);
	int y = igetarg_integer(it, 1);
	int i = igetarg_integer(it, 2);
	int j = igetarg_integer(it, 3);
	int c = igetarg_integer(it, 4);
	dsp_line(x, y, i, j, c);
	return 0;
}

int script_ppos(interpreter *it)
{
	dsp_cpos(0, 0);
	dsp_coff(igetarg_integer(it, 0), igetarg_integer(it, 1));
	return 0;
}

int script_color(interpreter *it)
{
	uint16_t c = dsp_color(igetarg_integer(it, 0), igetarg_integer(it, 1),
		igetarg_integer(it, 2));
	variable *v = make_varn(0, (float)c);
	iret(it, v);
	free(v);
	return 0;
}

int script_rand(interpreter *it)
{
	unsigned int mod = igetarg_integer(it, 0);
	unsigned int val = random_get();
	variable *v = make_varn(0, (float)(mod % val));
	iret(it, v);
	free(v);
	return 0;
}

int script_getkey(interpreter *it)
{
	variable *v = make_varn(0, (float)keypad_get());
	iret(it, v);
	free(v);
	return 0;
}

int script_pixel(interpreter *it)
{
	dsp_pixel(igetarg_integer(it, 0), igetarg_integer(it, 1),
		igetarg_integer(it, 2));
	return 0;
}

int script_solve(interpreter *it)
{
	const char *expr = igetarg_string(it, 0);
	int len = strlen(expr);
	char *buf = (char *)malloc(len + 2);
	strcpy(buf, expr);
	buf[len] = ')';
	buf[len + 1] = '\0';
	variable *r = idoexpr(it, buf);
	if (r == 0)
		r = make_varn(0, 0.0f);
	iret(it, r);
	free(r);
	return 0;
}
