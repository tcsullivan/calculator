#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

#define LCD_WIDTH  480
#define LCD_HEIGHT 320

void dsp_write_data(uint8_t data);
void dsp_set_addr(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void dsp_init(void);

#endif // DISPLAY_H_
