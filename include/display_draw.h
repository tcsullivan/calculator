#ifndef DISPLAY_DRAW_H_
#define DISPLAY_DRAW_H_

#include <stdint.h>

void dsp_cursoron(void);

void dsp_pixel(int x, int y, uint16_t color);
void dsp_line(int x, int y, int i, int j, uint16_t color);
void dsp_rect(int x, int y, int w, int h, uint16_t color);

void dsp_cpos(int x, int y);
void dsp_coff(int x, int y);
void dsp_puts(const char *s);

#endif // DISPLAY_DRAW_H_
