/**
 * @file serial.h
 * Provides basic serial IO (through STM debug stuff)
 */

#ifndef SERIAL_H_
#define SERIAL_H_

/**
 * Initializes the serial device.
 */
void serial_init(void);

/**
 * Puts the given character through serial.
 * @param c the character to send
 */
void serial_put(int c);

/**
 * Gets a character from serial.
 * @return the character
 */
char serial_get(void);

/**
 * Gets a string from serial, cut off by a newline.
 * @param buf the initialized buffer to fill
 * @param max the max amount of bytes to write to the buffer
 */
void serial_gets(char *buf, int max);

#endif // SERIAL_H_
