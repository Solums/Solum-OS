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

#include <stdint.h>
#include "drivers/port/port.h"
#include "drivers/port/serial.h"

#define UART_DR    0x00
#define UART_FR    0x18

#define UART_IBRD (0x24)  
#define UART_FBRD (0x28) 
#define UART_LCRH (0x2C)  
#define UART_CR   (0x30)  
#define UART_IMSC (0x38)  
#define UART_ICR  (0x44)  
#define UART_FR_TXFF (1 << 5)

static inline uint32_t uart_read32(uintptr_t base, uint32_t off) {
    return mmio_read32((uint64_t)(base + off));
}

static inline void uart_write32(uintptr_t base, uint32_t off, uint32_t val) {
    mmio_write32((uint64_t)(base + off), val);
}

void serial_init(uintptr_t port) {
    volatile uintptr_t* uart = (volatile uintptr_t*)port;

    uart[UART_CR / 4] = 0;
    uart[UART_IBRD / 4] = 1;
    uart[UART_FBRD / 4] = 40;
    uart[UART_LCRH / 4] = (1 << 4) | (1 << 5) | (1 << 6);
    uart[UART_CR / 4] = (1 << 0) | (1 << 8) | (1 << 9);
    uart[UART_ICR / 4] = 0x7FF;
}

void serial_putc(char c, uintptr_t serial_port) {
    uintptr_t base = serial_port;

    while (uart_read32(base, UART_FR) & UART_FR_TXFF) {}

    if (c == '\n') {
        uart_write32(base, UART_DR, '\r');
    }
    uart_write32(base, UART_DR, (uint32_t)c);
}

void serial_write(const char *buf, size_t len, uintptr_t serial_port) {
    for (size_t i = 0; i < len; i++) {
        serial_putc(buf[i], serial_port);
    }
}
