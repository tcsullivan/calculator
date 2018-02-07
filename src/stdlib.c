#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include <heap.h>

extern char *itoa(int, char *, int);

void _exit(int code)
{
	(void)code;
	for (;;);
}

int ksnprintf(char *buf, unsigned int count, const char *format, ...)
{
	(void)count;

	va_list args;
	va_start(args, format);

	unsigned int i = 0, o = 0;
	while (o < count && format[i] != '\0') {
		if (format[i] == '%') {
			if (format[i + 1] == 'd') {
				char *s = itoa(va_arg(args, int), malloc(16), 10);
				strncpy(buf + o, s, count - o);
				o += strlen(s);
				free(s);
			} else if (format[i + 1] == 'f') {
				strncpy(buf + o, "float", count - o);
				o += 5;
			}
			i++;
		} else {
			buf[o++] = format[i];
		}

		i++;
	}

	return o;
}

