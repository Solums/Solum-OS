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

#include "drivers/port/port.h"
#include "drivers/port/serial.h"
#include <stdint.h>

#define UART_TX    0x00
#define UART_LSR   0x05

#define UART_LSR_THRE (1 << 5)

static inline uint8_t uart_read8(uintptr_t base, uint32_t off) {
    return mmio_read8((uint64_t)(base + off));
}

static inline void uart_write8(uintptr_t base, uint32_t off, uint8_t val) {
    mmio_write8((uint64_t)(base + off), val);
}

void serial_init(uintptr_t serial_port) {
    (void)serial_port;
}

void serial_putc(char c, uintptr_t serial_port) {
    uintptr_t base = serial_port;

    while (!(uart_read8(base, UART_LSR) & UART_LSR_THRE)) {}

    if (c == '\n') {
        uart_write8(base, UART_TX, '\r');
    }
    uart_write8(base, UART_TX, (uint8_t)c);
}

void serial_write(const char *buf, size_t len, uintptr_t serial_port) {
    for (size_t i = 0; i < len; i++) {
        serial_putc(buf[i], serial_port);
    }
}
