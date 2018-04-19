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

#include <it/builtins.h>
#include <clock.h>
#include <ctype.h>
#include <display.h>
#include <display_draw.h>
#include <display_text.h>
#include <heap.h>
#include <initrd.h>
#include <it/string.h>
#include <keypad.h>
#include <math.h>
#include <random.h>
#include <serial.h>
#include <stdlib.h>

#define igetarg_integer(it, n) ((int)igetarg(it, n)->value.f)

int script_puts(instance *it);
int script_putchar(instance *it);
int script_gets(instance *it);
int script_getf(instance *it);
int script_delay(instance *it);
int script_rect(instance *it);
int script_ppos(instance *it);
int script_rpos(instance *it);
int script_line(instance *it);
int script_color(instance *it);
int script_rand(instance *it);
int script_getkey(instance *it);
int script_pixel(instance *it);
int script_menu(instance *it);
int script_filemenu(instance *it);
int script_program(instance *it);
int script_free(instance *it);
int script_clear(instance *it);

int math_sin(instance *it);
int math_cos(instance *it);
int math_tan(instance *it);

void script_loadlib(instance *it)
{
	inew_number(it, "pi", 3.1415926535f);

	inew_cfunc(it, "print", script_puts);
	inew_cfunc(it, "putchar", script_putchar);
	inew_cfunc(it, "gets", script_gets);
	inew_cfunc(it, "getf", script_getf);
	inew_cfunc(it, "getkey", script_getkey);
	inew_cfunc(it, "ppos", script_ppos);
	inew_cfunc(it, "rpos", script_rpos);

	inew_cfunc(it, "pixel", script_pixel);
	inew_cfunc(it, "line", script_line);
	inew_cfunc(it, "rect", script_rect);
	inew_cfunc(it, "color", script_color);
	inew_cfunc(it, "clear", script_clear);

	inew_cfunc(it, "rand", script_rand);
	inew_cfunc(it, "delay", script_delay);

	inew_cfunc(it, "menu", script_menu);
	inew_cfunc(it, "filemenu", script_filemenu);
	inew_cfunc(it, "program", script_program);
	inew_cfunc(it, "freemem", script_free);

	inew_cfunc(it, "sin", math_sin);
	inew_cfunc(it, "cos", math_cos);
	inew_cfunc(it, "tan", math_tan);
}

int math_sin(instance *it)
{
	variable *n = igetarg(it, 0);
	variable *v = make_varf(0, sinf(n->value.f));
	ipush(it, (uint32_t)v);
	return 0;
}

int math_cos(instance *it)
{
	variable *n = igetarg(it, 0);
	variable *v = make_varf(0, cosf(n->value.f));
	ipush(it, (uint32_t)v);
	return 0;
}

int math_tan(instance *it)
{
	variable *n = igetarg(it, 0);
	variable *v = make_varf(0, tanf(n->value.f));
	ipush(it, (uint32_t)v);
	return 0;
}

int script_menu(instance *it)
{
	char listbuf[4];
	int nargs = igetarg_integer(it, 0);
	float *resps = (float *)calloc(nargs, sizeof(float));
	strncpy(listbuf, " : \0", 4);

	text_switch(1);
	text_clear();

	for (int i = 0; i < nargs; i++) {
		listbuf[0] = i + '0';
		text_puts(listbuf);
		text_puts((char *)igetarg(it, 1 + i * 2)->value.p);
		text_puts("\n");
		resps[i] = igetarg(it, 2 + i * 2)->value.f;
	}

	int c;
	do c = keypad_get();
	while (c == 0);

	variable *v = make_varf(0, isdigit(c) ? c - '0' : -1.0f);
	ipush(it, (uint32_t)v);

	text_switch(0);
	free(resps);
	return 0;
}

#include <fat32.h>

int script_filemenu(instance *it)
{
	char listbuf[4];
	char *fname;
	strncpy(listbuf, " : \0", 4);
	text_puts("Choose a file: \n");
	for (unsigned int i = 0; (fname = /*initrd*/fat_getname(i)) != 0; i++) {
		listbuf[0] = i + '0';
		text_puts(listbuf);
		text_puts(fname);
		free(fname);
		text_puts("\n");
	}

	int c;
	do c = keypad_get();
	while (c == 0);

	variable *v = make_varf(0, isdigit(c) ? c - '0' : -1.0f);
	ipush(it, (uint32_t)v);

	return 0;
}

int script_puts(instance *it)
{
	variable *v = igetarg(it, 0);
	if (v->type == NUMBER) {
		char buf[33];
		//snprintf(buf, 33, "%d", (int)v->value.f); // TODO
		ftostr(buf, v->value.f);
		text_puts(buf);
	} else if (v->type == STRING) {
		text_puts((const char *)v->value.p);
	}
	return 0;
}

int script_putchar(instance *it)
{
	variable *v = igetarg(it, 0);
	char buf[2];
	
	buf[0] = (int)v->value.f;
	buf[1] = '\0';
	text_puts(buf);

	return 0;
}

int script_getf(instance *it)
{
	if (script_gets(it) != 0)
		return -1;

	variable *s = (variable *)ipop(it);
	s->value.f = strtof((char *)s->value.p, 0);
	s->type = NUMBER;
	ipush(it, (uint32_t)s);
	return 0;
}

int script_gets(instance *it)
{
	char *s = malloc(64);
	char c[2] = {0, 0};
	int index = 0;
	int furthest = 0;

	do {
		do {
			c[0] = keypad_get();
			delay(1);
		} while (c[0] == 0);

		if (c[0] == 0x7F) {
			it->lnidx = 998;
			break;
		} else if (c[0] == K_LEFT) {
			if (index > 0) {
				text_relpos(-1, 0);
				index--;
			}
			continue;
		} else if (c[0] == K_RIGHT) {
			if (index < furthest) {
				text_relpos(1, 0);
				index++;
			}
			continue;
		} else if (c[0] == K_UP || c[0] == K_DOWN)
			continue;

		if (c[0] == '\n') {
			s[furthest] = '\n';
			break;
		}

		extern int keypad_insert;
		if (keypad_insert != 0 && index < furthest) {
			for (int i = furthest; i >= index; i--)
				s[i] = s[i - 1];
		}

		s[index] = c[0];
		if (c[0] == '\b' || c[0] == 127) {
			index--;
			if (index > -1) {
				text_puts("\b");
				index--;
			}
		} else if (keypad_insert != 0) {
			text_relpos(-index, 0);
			s[furthest + 1] = '\0';
			text_puts(s);
			text_relpos(-(furthest - index), 0);
			furthest++;
		} else if (c[0] != '\n'/*'\r'*/) {
			text_puts(c);
		}

		if (++index > furthest)
			furthest = index;
	} while (furthest < 63);
	s[furthest] = '\0';

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
	text_setpos(igetarg_integer(it, 0), igetarg_integer(it, 1));
	return 0;
}

int script_rpos(instance *it)
{
	text_relpos(igetarg_integer(it, 0), igetarg_integer(it, 1));
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
	char c = keypad_get();
	if (c == 0x7F)
		it->lnidx = 998;
	variable *v = make_varf(0, c);
	ipush(it, (uint32_t)v);
	return 0;
}

int script_pixel(instance *it)
{
	dsp_pixel(igetarg_integer(it, 0), igetarg_integer(it, 1),
		igetarg_integer(it, 2));
	return 0;
}

extern instance *load_program(const char *name);
int script_program(instance *it)
{
	int initrdOffset = (int)igetarg(it, 0)->value.f;
	char *name = fat_getname(initrdOffset);

	text_clear();
	instance *it2 = load_program(name);
	free(name);

	int ret = irun(it2);
	if (ret != 0)
		return -1;

	idelinstance(it2);
	return 0;
}

int script_free(instance *it)
{
	extern uint32_t heap_used;
	ipush(it, (uint32_t)make_varf(0, 98303 - heap_used));
	return 0;
}

int script_clear(instance *it)
{
	(void)it;
	text_clear();
	return 0;
}
