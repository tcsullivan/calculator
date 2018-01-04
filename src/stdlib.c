#include <stdint.h>

void _exit(int code)
{
	(void)code;
	for (;;);
}

char *itoa(int n, int base)
{
	static char buf[16];
	char *p = buf + 15;
	*p = '\0';
	do *--p = "0123456789ABCDEF"[n % base];
	while (n /= base);
	return p;
}
