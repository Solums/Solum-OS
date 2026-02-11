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

void serial_init(uintptr_t serial_port)
{
    uint16_t base = (uint16_t)serial_port;
    outb(base + 1, 0x00); // Disable all interrupts
    outb(base + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(base + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(base + 1, 0x00); //                  (hi byte)
    outb(base + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(base + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(base + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

void serial_putc(char c, uintptr_t serial_port)
{
    uint16_t port = (uint16_t)serial_port;
    if (c == '\n') {
        outb(port, '\r');
    }
    outb(port, c);
}

void serial_write(const char *buf, size_t len, uintptr_t serial_port)
{
    for (size_t i = 0; i < len; i++) {
        serial_putc(buf[i], serial_port);
    }
}