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

#include "drivers/tty.h"
#include "kernel/msg/vsnprintk.h"
#include "kernel/msg/printk.h"

int printk(const char *format, ...)
{
    if (!format) return 0;

    static char kbuf[4096];

    va_list args;
    va_start(args, format);

    int len = vsnprintk(kbuf, sizeof(kbuf), format, args);
    tty_write(kbuf, len);

    va_end(args);
    return len;
}