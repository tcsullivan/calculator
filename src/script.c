#include <script.h>

#include <builtins.h>
#include <clock.h>
#include <display.h>
#include <display_draw.h>
#include <heap.h>
#include <random.h>
#include <serial.h>
#include <stack.h>

int script_puts(interpreter *it);
int script_gets(interpreter *it);
int script_delay(interpreter *it);
int script_rect(interpreter *it);
int script_ppos(interpreter *it);
int script_line(interpreter *it);
int script_color(interpreter *it);
int script_rand(interpreter *it);

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
	variable v;
	v.valtype = INTEGER;
	INT(&v) = c;
	v.svalue = 0;
	isetstr(&v);
	iret(it, &v);
	return 0;
}

int script_rand(interpreter *it)
{
	variable *v = (variable *)malloc(sizeof(variable));
	unsigned int mod = igetarg_integer(it, 0);
	unsigned int val = random_get();
	v->valtype = INTEGER;
	INT(v) = val % mod;
	v->svalue = 0;
	isetstr(v);
	iret(it, v);
	free(v->svalue);
	free(v);
	return 0;
}
