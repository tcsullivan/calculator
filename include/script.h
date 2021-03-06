/**
 * @file script.h
 * Provides script library for using calculator hardware
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

#ifndef SCRIPT_H_
#define SCRIPT_H_

#include <it/parser.h>

/**
 * Loads the library for the given interpreter instance.
 * @param it the interpreter to use
 */
void script_loadlib(instance *it);

#endif // SCRIPT_H_
