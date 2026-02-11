/*
 * Copyright (C) 2025 Roy Roy123ty@hotmail.com
 * 
 * This file is part of Solum OS
 * 
 * It is free software licensed under the
 * GNU General Public License version 3 (GPLv3).
 * 
 * This file is based on the ViudiraTech Uinxed-Kernel project
 * (https://github.com/ViudiraTech/Uinxed-Kernel), which was licensed under the
 * Apache License, Version 2.0.
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

#ifndef MATH_H
#define MATH_H

#include <stdint.h>

#define PI            (float64_t)3.1415926535
#define TAU           (float64_t)6.2831853071
#define PI_DIV_2      (float64_t)1.5707963267
#define LONG_LONG_MAX (0x7FFFFFFFFFFFFFFFLL)
#define LONG_LONG_MIN (-0x7FFFFFFFFFFFFFFFLL - 1)
#define SHRT_MAX      (0x7FFF)
#define SHRT_MIN      (-0x7FFF - 1)
#define INT_MAX       (0x7FFFFFFF)
#define INT_MIN       (-0x7FFFFFFF - 1)
#define UINT_MAX      (0xFFFFFFFFU)
#define UINT_MIN      (0x00000000U)
#define LONG_MAX      (0x7FFFFFFFFFFFFFFFL)
#define LONG_MIN      (-0x7FFFFFFFFFFFFFFFL - 1)

#define FORCE_EVAL(x)                                         \
    do {                                                      \
        if (sizeof(x) == sizeof(float)) {                     \
            volatile float __x __attribute__((unused));       \
            __x = (x);                                        \
        } else if (sizeof(x) == sizeof(double)) {             \
            volatile double __x __attribute__((unused));      \
            __x = (x);                                        \
        } else {                                              \
            volatile long double __x __attribute__((unused)); \
            __x = (x);                                        \
        }                                                     \
    } while (0)

static const double rounders[10 + 1] = {
    0.5,          // 01 decimal place
    0.05,         // 02 decimal place
    0.005,        // 03 decimal place
    0.0005,       // 04 decimal place
    0.00005,      // 05 decimal place
    0.000005,     // 06 decimal place
    0.0000005,    // 07 decimal place
    0.00000005,   // 08 decimal place
    0.000000005,  // 09 decimal place
    0.0000000005, // 10 decimal place
    0.0000000000  // 11 decimal place
};

#endif
