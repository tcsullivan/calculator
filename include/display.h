/*
 * @file display.h
 * Display library for ILI9481 display.
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

/**
 * The screen's width, in pixels.
 */
#define LCD_WIDTH  480

/**
 * The screen's height, in pixels.
 */
#define LCD_HEIGHT 320

/**
 * Returns the color integer for the given RGB values.
 * Converts 8RGB to 5-6-5.
 * @param r red value, 0-255
 * @param g green value, 0-255
 * @param b blue value, 0-255
 * @return the 5-6-5 color value
 */
uint16_t dsp_color(uint8_t r, uint8_t g, uint8_t b);

/**
 * Sets the direction of IO, for reading or writing to the screen.
 * @param mode INPUT or OUPUT (defined in gpio.h)
 */
void dsp_dmode(int mode);

/**
 * Writes the command byte to the display.
 * @param data the command to write
 */
void dsp_write_cmd(uint8_t data);

/**
 * Writes the data byte to the display.
 * @param data the data to write
 */
void dsp_write_data(uint8_t data);

/**
 * Reads a byte of data from the display.
 * @return the data byte
 */
uint8_t dsp_read_data(void);

void dsp_set_addr(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void dsp_set_addr_read(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void dsp_init(void);

#endif // DISPLAY_H_
