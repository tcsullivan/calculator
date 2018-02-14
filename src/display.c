#include <display.h>

#include <clock.h>
#include <gpio.h>

#include <stdint.h>

#define LCD_RD  GPIO_PORT(A, 0)
#define LCD_WR  GPIO_PORT(A, 1)
#define LCD_RS  GPIO_PORT(A, 4)
#define LCD_CS  GPIO_PORT(B, 0)
#define LCD_RST GPIO_PORT(C, 1)
#define LCD_D0  GPIO_PORT(A, 9)
#define LCD_D1  GPIO_PORT(C, 7)
#define LCD_D2  GPIO_PORT(A, 10)
#define LCD_D3  GPIO_PORT(B, 3)
#define LCD_D4  GPIO_PORT(B, 5)
#define LCD_D5  GPIO_PORT(B, 4)
#define LCD_D6  GPIO_PORT(B, 10)
#define LCD_D7  GPIO_PORT(A, 8)

// bbbbbggg gggrrrrr 

void dsp_write_data(uint8_t data)
{
	gpio_dout(LCD_D0, data & 0x01);
	gpio_dout(LCD_D1, data & 0x02);
	gpio_dout(LCD_D2, data & 0x04);
	gpio_dout(LCD_D3, data & 0x08);
	gpio_dout(LCD_D4, data & 0x10);
	gpio_dout(LCD_D5, data & 0x20);
	gpio_dout(LCD_D6, data & 0x40);
	gpio_dout(LCD_D7, data & 0x80);
	gpio_dout(LCD_WR, 0);
	gpio_dout(LCD_WR, 1);
}

void dsp_write_cmd(uint8_t data)
{
	gpio_dout(LCD_RS, 0);
	dsp_write_data(data);
	gpio_dout(LCD_RS, 1);
}

void dsp_set_addr(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	dsp_write_cmd(0x2A);
	dsp_write_data(x1 >> 8);
	dsp_write_data(x1 & 0xFF);
	dsp_write_data(x2 >> 8);
	dsp_write_data(x2 & 0xFF);
	dsp_write_cmd(0x2B);
	dsp_write_data(y1 >> 8);
	dsp_write_data(y1 & 0xFF);
	dsp_write_data(y2 >> 8);
	dsp_write_data(y2 & 0xFF);
	dsp_write_cmd(0x2C); // begin writing
}

void dsp_init(void)
{
	gpio_mode(LCD_CS, OUTPUT);
	gpio_mode(LCD_RS, OUTPUT);
	gpio_mode(LCD_RD, OUTPUT);
	gpio_mode(LCD_WR, OUTPUT);
	gpio_mode(LCD_RST, OUTPUT);
	gpio_mode(LCD_D0, OUTPUT);
	gpio_mode(LCD_D1, OUTPUT);
	gpio_mode(LCD_D2, OUTPUT);
	gpio_mode(LCD_D3, OUTPUT);
	gpio_mode(LCD_D4, OUTPUT);
	gpio_mode(LCD_D5, OUTPUT);
	gpio_mode(LCD_D6, OUTPUT);
	gpio_mode(LCD_D7, OUTPUT);
	gpio_dout(LCD_CS, 1);
	gpio_dout(LCD_RS, 1);
	gpio_dout(LCD_RD, 1);
	gpio_dout(LCD_WR, 1);
	gpio_dout(LCD_RST, 1);
	gpio_dout(LCD_D0, 0);
	gpio_dout(LCD_D1, 0);
	gpio_dout(LCD_D2, 0);
	gpio_dout(LCD_D3, 0);
	gpio_dout(LCD_D4, 0);
	gpio_dout(LCD_D5, 0);
	gpio_dout(LCD_D6, 0);
	gpio_dout(LCD_D7, 0);

	delay(50);
	gpio_dout(LCD_RST, 0);
	delay(15);
	gpio_dout(LCD_RST, 1);
	delay(15);

	dsp_write_cmd(0x01); // soft reset
	delay(50);

	dsp_write_cmd(0xD0); // power setting
	dsp_write_data(0x07);
	dsp_write_data(0x42);
	dsp_write_data(0x18);
	dsp_write_cmd(0xD1); // vcom control
	dsp_write_data(0x00);
	dsp_write_data(0x07);
	dsp_write_data(0x10);
	dsp_write_cmd(0xD2); // power for normal mode
	dsp_write_data(0x01);
	dsp_write_data(0x02);
	dsp_write_cmd(0xD3);
	dsp_write_data(0x01);
	dsp_write_data(0x02);
	dsp_write_cmd(0xD4);
	dsp_write_data(0x01);
	dsp_write_data(0x02);
	dsp_write_cmd(0xC0); // panel driving setting
	dsp_write_data(0x10);
	dsp_write_data(0x3B);
	dsp_write_data(0x00);
	dsp_write_data(0x02);
	dsp_write_data(0x11);
	dsp_write_cmd(0xC5); // frame rate/inversion ctl
	dsp_write_data(0x03);
	dsp_write_cmd(0x36); // rot. and stuff
	dsp_write_data(0x41);
	dsp_write_cmd(0x3A); // set pixel format
	dsp_write_data(0x55);
	dsp_write_cmd(0x11);
	delay(150);
	dsp_write_cmd(0x29); // set display on
	delay(500);
	dsp_write_cmd(0x33); // set scroll area
	dsp_write_data(0x00);
	dsp_write_data(0x00);
	dsp_write_data(LCD_HEIGHT >> 8);
	dsp_write_data((LCD_HEIGHT & 0xFF) - 1);
	dsp_write_data(0x00);
	dsp_write_data(0x00);
	dsp_write_cmd(0x37);
	dsp_write_data(0x00);
	dsp_write_data(0x00);

	dsp_set_addr(0, 0, LCD_WIDTH, LCD_HEIGHT);
}

