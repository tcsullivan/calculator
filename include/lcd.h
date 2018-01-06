#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

/**
 * Direct access
 */
void lcd_init(void);

void lcd_puts(const char *s);
void lcd_puti(int i);
void lcd_puth(int h);
void lcd_putb(uint8_t b);
void lcd_clear(void);

/**
 * Buffered/async access
 */
void lcd_handler(void);
void lcd_put(const char *s);

#endif // LCD_H_
