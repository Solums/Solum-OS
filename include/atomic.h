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

#ifndef ATOMIC_H
#define ATOMIC_H

#include <stdint.h>

#ifdef __aarch64__
static inline int atomic_exchange_int(volatile int *ptr, int val) {
    int old;
    unsigned int res;
    asm volatile(
        "1: ldaxr %w0, [%2]\n"
        "   stlxr %w1, %w3, [%2]\n"
        "   cbnz %w1, 1b\n"
        : "=&r" (old), "=&r" (res)
        : "r" (ptr), "r" (val)
        : "memory");
    return old;
}

static inline void atomic_store_release_int(volatile int *ptr, int val) {
    asm volatile("stlr %w1, [%0]" : : "r" (ptr), "r" (val) : "memory");
}

#else
static inline int atomic_exchange_int(volatile int *ptr, int val) {
    return __atomic_exchange_n(ptr, val, __ATOMIC_ACQ_REL);
}

static inline void atomic_store_release_int(volatile int *ptr, int val) {
    __atomic_store_n(ptr, val, __ATOMIC_RELEASE);
}
#endif

#endif
