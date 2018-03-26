/**
 * @file stdlib.h
 * Provides missing C standard library functions
 * The newlib versions of these calls are not being used due to dependence on
 * unsupported system calls.
 *
 * Copyright (C) 2018 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef STDLIB_H_
#define STDLIB_H_

/**
 * Parses a formatted string, storing the result in the given buffer.
 * @param buf the buffer to print the parsed string to
 * @param max the maximum number of bytes to write to the buffer
 * @param format the format string
 * @return a pointer to the buffer
 */
char *snprintf(char *buf, unsigned int max, const char *format, ...);

/**
 * Attempts to convert the given string to a float.
 * @param s the string to convert
 * @param endptr if not-null, set to the address of the character after the
 * number
 * @return the resulting float, or zero if an error occurred
 */
float strtof(const char *s, char **endptr);

/**
 * Attempts to convert the given string to an integer.
 * @param s the string to convert
 * @return the resulting integer, or zero if an error occurred
 */
int atoi(const char *s);

#endif // STDLIB_H_
