/**
 * @file script.h
 * Provides script library for using calculator hardware
 */

#ifndef SCRIPT_H_
#define SCRIPT_H_

#include <parser.h>

/**
 * Loads the library for the given interpreter.
 * @param it the interpreter to use
 */
void script_loadlib(instance *it);

#endif // SCRIPT_H_
