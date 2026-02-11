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

#include "libc/stdlib.h"
#include "libc/math.h"

int atoi(const char *str) {
    int result = 0;
    int sign = 1;
    
    while (IS_SPACE(*str)) {
        str++;
    }
    
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }
    
    while (*str >= '0' && *str <= '9') {
        if (result > (INT_MAX / 10) || 
            (result == (INT_MAX / 10) && (*str - '0') > (INT_MAX % 10))) {
            return sign == 1 ? INT_MAX : INT_MIN;
        }
        
        result = result * 10 + (*str - '0');
        str++;
    }
    
    return sign * result;
}

void itoa(int64_t value, char *str, unsigned base, size_t size) {
    if (!str || size <= 0) return;
    if (base < 2 || base > 36) {
        str[0] = '\0';
        return;
    }

    static char buf[68];
    int i = 0;
    unsigned negative = 0;
    uint64_t u;

    if (value < 0 && base == 10) {
        negative = 1;
        if (value == INT64_MIN) {
            u = (uint64_t)INT64_MAX + 1;
        } else {
            u = (uint64_t)(-value);
        }
    } else {
        u = (uint64_t)value;
    }

    if (u == 0) {
        buf[i++] = '0';
    } else {
        while (u != 0) {
            int rem = u % base;
            buf[i++] = (rem > 9) ? ('A' + (rem - 10)) : ('0' + rem);
            u /= base;
        }
    }

    if (base == 16) {
        buf[i++] = 'x';
        buf[i++] = '0';
    } else if (base == 8) {
        buf[i++] = '0';
    }

    if (negative) {
        buf[i++] = '-';
    }

    int j = 0;
    while (i > 0) {
        i--;
        str[j++] = buf[i];
    }
    str[j] = '\0';
}

void uitoa(uint64_t value, char *str, unsigned base, size_t size) 
{
    if (!str || size <= 0) return;
    if (base < 2 || base > 36) {
        str[0] = '\0';
        return;
    }

    static char buf[68];
    int i = 0;

    if (value == 0) {
        buf[i++] = '0';
    } else {
        while (value != 0) {
            int rem = value % base;
            buf[i++] = (rem > 9) ? ('A' + (rem - 10)) : ('0' + rem);
            value /= base;
        }
    }

    if (base == 16) {
        buf[i++] = 'x';
        buf[i++] = '0';
    } else if (base == 8) {
        buf[i++] = '0';
    }

    int j = 0;
    while (i > 0) {
        i--;
        str[j++] = buf[i];
    }
    str[j] = '\0';
}