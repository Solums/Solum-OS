/*
 * Copyright (C) 2025 Roy Roy123ty@hotmail.com
 * 
 * This file is part of Solum OS
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TTY_H
#define TTY_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define TTY_LINE_BUF  256

void tty_init();
int tty_write(const char *buf, size_t len);
int tty_putc(char c);
int tty_puts(const char *s);
void tty_flush();
size_t tty_buffer_usage();

#endif