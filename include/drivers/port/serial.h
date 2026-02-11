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

#ifndef SERIAL_H
#define SERIAL_H

#include <stddef.h>
#include <stdint.h>

#if defined(__x86_64__)

#define SERIAL_COM1 0x3F8
#define SERIAL_COM2 0x2F8
#define SERIAL_COM3 0x3E8
#define SERIAL_COM4 0x2E8

#elif defined(__aarch64__)

#define SERIAL_COM1 0x09000000U
#define SERIAL_COM2 0x00000000U
#define SERIAL_COM3 0x00000000U
#define SERIAL_COM4 0x00000000U

#elif defined(__riscv) && __riscv_xlen == 64

#define SERIAL_COM1 0x10000000U
#define SERIAL_COM2 0x00000000U
#define SERIAL_COM3 0x00000000U
#define SERIAL_COM4 0x00000000U

#endif

#define ANSI_RED    "\x1b[31m"  
#define ANSI_GREEN  "\x1b[32m"  
#define ANSI_YELLOW "\x1b[33m"  
#define ANSI_BLUE   "\x1b[34m"
#define ANSI_DEFA   "\x1b[0m"

void serial_init(uintptr_t serial_port);
void serial_putc(char c, uintptr_t serial_port);
void serial_write(const char *buf, size_t len, uintptr_t serial_port);

#endif
