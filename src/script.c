/**
 * @file script.c
 * Provides script library for using calculator hardware
 *
 * Copyright (C) 2018 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <script.h>

#include <builtins.h>
#include <clock.h>
#include <display.h>
#include <display_draw.h>
#include <heap.h>
#include <initrd.h>
#include <random.h>
#include <serial.h>
#include <stdlib.h>
#include <keypad.h>

#include <string.h>

#define igetarg_integer(it, n) ((int)igetarg(it, n)->value.f)

int script_puts(instance *it);
int script_putchar(instance *it);
int script_gets(instance *it);
int script_delay(instance *it);
int script_rect(instance *it);
int script_ppos(instance *it);
int script_line(instance *it);
int script_color(instance *it);
int script_rand(instance *it);
int script_getkey(instance *it);
int script_pixel(instance *it);
int script_menu(instance *it);
int script_filemenu(instance *it);

void script_loadlib(instance *it)
{
	inew_cfunc(it, "print", script_puts);
	inew_cfunc(it, "putchar", script_putchar);
	inew_cfunc(it, "gets", script_gets);
	inew_cfunc(it, "getkey", script_getkey);
	inew_cfunc(it, "ppos", script_ppos);

	inew_cfunc(it, "pixel", script_pixel);
	inew_cfunc(it, "line", script_line);
	inew_cfunc(it, "rect", script_rect);
	inew_cfunc(it, "color", script_color);

	inew_cfunc(it, "rand", script_rand);
	inew_cfunc(it, "delay", script_delay);

	inew_cfunc(it, "menu", script_menu);
	inew_cfunc(it, "filemenu", script_filemenu);
}

int script_menu(instance *it)
{
	char listbuf[4];
	int nargs = igetarg_integer(it, 0);
	float *resps = (float *)calloc(nargs, sizeof(float));
	strncpy(listbuf, " : \0", 4);
	for (int i = 0; i < nargs; i++) {
		listbuf[0] = i + '0';
		dsp_puts(listbuf);
		dsp_puts((char *)igetarg(it, 1 + i * 2)->value.p);
		dsp_puts("\n");
		resps[i] = igetarg(it, 2 + i * 2)->value.f;
	}
	free(resps);
	return 0;
}

int script_filemenu(instance *it)
{
	char listbuf[4];
	char *buf = calloc(17, 1);
	char *fname;
	strncpy(listbuf, " : \0", 4);
	dsp_puts("Choose a file: \n");
	for (unsigned int i = 0; (fname = initrd_getnfile(i)) != 0; i++) {
		listbuf[0] = i + '0';
		dsp_puts(listbuf);
		dsp_puts(strncpy(buf, fname, 16));
		dsp_puts("\n");
	}
	free(buf);
	return script_gets(it);
}

int script_puts(instance *it)
{
	variable *v = igetarg(it, 0);
	if (v->type == NUMBER) {
		char buf[33];
		snprintf(buf, 33, "%d", (int)v->value.f); // TODO
		dsp_puts(buf);
	} else if (v->type == STRING) {
		dsp_puts((const char *)v->value.p);
	}
	return 0;
}

int script_putchar(instance *it)
{
	variable *v = igetarg(it, 0);
	char buf[2];
	
	buf[0] = (int)v->value.f;
	buf[1] = '\0';
	dsp_puts(buf);

	return 0;
}

int script_gets(instance *it)
{
	char *s = malloc(64);
	char c[2] = {0, 0};
	int index = 0;

	do {
		do {
			c[0] = keypad_get();
			delay(1);
		} while (c[0] == 0);
		//c[0] = serial_get();
		s[index] = c[0];
		if (c[0] == '\b' || c[0] == 127) {
			index--;
			if (index > -1) {
				dsp_puts("\b");
				index--;
			}
		} else if (c[0] != '\n'/*'\r'*/) {
			dsp_puts(c);
		}
	} while (s[index] != '\n'/*'\r'*/ && index++ < 63);
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
	variable *v = make_varf(0, (float)(val % mod));
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

