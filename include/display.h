#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

#define LCD_WIDTH  320
#define LCD_HEIGHT 480

#define COLOR_MAX 31

uint16_t dsp_color(uint8_t r, uint8_t g, uint8_t b);
void dsp_write_cmd(uint8_t data);
void dsp_write_data(uint8_t data);
void dsp_set_addr(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void dsp_init(void);

#endif // DISPLAY_H_
