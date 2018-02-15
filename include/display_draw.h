#ifndef DISPLAY_DRAW_H_
#define DISPLAY_DRAW_H_

void dsp_rect(int x, int y, int w, int h, uint16_t color);

void dsp_cpos(int x, int y);
void dsp_puts(const char *s);

#endif // DISPLAY_DRAW_H_
