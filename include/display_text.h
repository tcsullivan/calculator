#ifndef DISPLAY_TEXT_H_
#define DISPLAY_TEXT_H_

void text_init(void);
void text_switch(unsigned int i);
void text_puts(const char *s);

void text_clear(void);

void text_setpos(uint8_t x, uint8_t y);
void text_relpos(int8_t x, int8_t y);

#endif // DISPLAY_TEXT_H_
