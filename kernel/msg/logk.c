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

#include "kernel/msg/printk.h"
#include "kernel/msg/vsnprintk.h"
#include "drivers/tty.h"
#include "solum.h"
#include "kernel/msg/logk.h"

static const char *level_tag(int level)
{
    switch (level) {
        case 0: return "[EMERG] ";
        case 1: return "[ALERT] ";
        case 2: return "[CRIT] ";
        case 3: return "[ERR] ";
        case 4: return "[WARN] ";
        case 5: return "[NOTICE] ";
        case 6: return "[INFO] ";
        case 7: return "[DEBUG] ";
        default: return "[INFO] ";
    }
}

static int printk_with_level(int level, const char *format, va_list args)
{
    static char kbuf[4096];
    const char *tag = level_tag(level);

    size_t tlen = strlen(tag);
    if (tlen >= sizeof(kbuf)) return 0;

    for (size_t i = 0; i < tlen; i++) kbuf[i] = tag[i];

    int msg_len = vsnprintk(kbuf + tlen, sizeof(kbuf) - tlen, format, args);
    if (msg_len < 0) msg_len = 0;

    size_t total = tlen + (size_t)msg_len;
    tty_write(kbuf, total);
    return (int)total;
}

void logk(const char *format, ...) 
{
    int level = 6;
    const char *p = format;
    
    if (*p == '<') {
        p++;
        int v = 0;
        bool got = false;
        while (*p >= '0' && *p <= '9') {
            got = true;
            v = v * 10 + (*p - '0');
            p++;
        }
        if (got && *p == '>') {
            level = v;
            p++;
            format = p; 
        }
    }

    if (level <= LOG_SHOW_LEVEL) {
        va_list args;
        va_start(args, format);
        printk_with_level(level, format, args);
        va_end(args);
    } else {
        return;
    }
    
}