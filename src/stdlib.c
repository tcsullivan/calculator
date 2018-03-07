#include <stdlib.h>

#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

extern char *itoa(int, char *, int);

void _exit(int code)
{
	(void)code;
	for (;;);
}

char *snprintf(char *buf, unsigned int max, const char *format, ...)
{
	va_list args;
	va_start(args, format);

	if (buf == 0 || max == 0)
		return 0;

	static char nbuf[32];
	unsigned int off = 0;
	for (unsigned int i = 0; format[i] != '\0' && off < max; i++) {
		if (format[i] == '%') {
			i++;
			nbuf[0] = '\0';
			switch (format[i]) {
			case 'd':
				itoa(va_arg(args, int), nbuf, 10);
				break;
			case 'u':
				itoa(va_arg(args, unsigned int), nbuf, 10);
				break;
			case 'x':
			case 'p':
				itoa(va_arg(args, unsigned int), nbuf, 16);
				break;
			case 'f':
				itoa((int)va_arg(args, double), nbuf, 10);
				continue;
				break;
			default:
				buf[off++] = format[i];
				nbuf[0] = '\0';
				break;
			}
			if (nbuf[0] != '\0') {
				for (unsigned int j = 0; off < max && j < 32 &&
					nbuf[j] != '\0'; off++, j++)
					buf[off] = nbuf[j];
			}
		} else {
			buf[off++] = format[i];
		}
	}


	buf[off >= max ? max - 1 : off] = '\0';
	va_end(args);
	return buf;
}

float strtof(const char *s, char **endptr)
{
	(void)s;
	(void)endptr;
	return 0.0f;
}

int atoi(const char *s)
{
	int n = 0;
	for (unsigned int i = 0; isdigit(s[i]); i++) {
		n *= 10;
		n += s[i] - '0';
	}
	return n;
}
