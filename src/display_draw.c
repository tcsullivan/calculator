#include <display.h>

static unsigned int curx = 0;
static unsigned int cury = 0;

extern const unsigned char inconsolata24[192 * 156 * 2 + 1];

void dsp_putchar(int c)
{
	unsigned int start = ((c - ' ') / 16 * 192 * 26 + (c % 16) * 12) * 2;

	dsp_set_addr(curx * 12, cury * 26, curx * 12 + 11, cury * 26 + 25);
	// for each row
	for (unsigned int i = 0; i < 26; i++) {
		// for each column
		for (int j = 12 * 2 - 1; j >= 0; j--)
			dsp_write_data(inconsolata24[start + (i * 192 * 2) + j]);
	}

	if (++curx == 40) {
		curx = 0;
		if (++cury == 10)
			cury = 0;
	}
}

void dsp_puts(const char *s)
{
	unsigned int i = 0;
	while (s[i])
		dsp_putchar(s[i++]);
}

void dsp_cpos(int x, int y)
{
	curx = x;
	cury = y;
}

void dsp_rect(int x, int y, int w, int h, uint16_t color)
{
	dsp_set_addr(x, y, x + w - 1, y + h - 1);
	int countdown = w * h;
	do {
		dsp_write_data(color >> 8);
		dsp_write_data(color & 0xFF);
	} while (countdown--);
}
