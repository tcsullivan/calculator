#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

void lcd_init(void);

void lcd_puts(const char *s);
void lcd_puti(int i);
void lcd_puth(int h);
void lcd_putb(uint8_t b);
void lcd_clear(void);

#endif // LCD_H_
