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

#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>
#include <stddef.h>

#define VIDEO_BLACK         0x000000
#define VIDEO_WHITE         0xFFFFFF
#define VIDEO_RED           0xFF0000
#define VIDEO_GREEN         0x00FF00
#define VIDEO_BLUE          0x0000FF
#define VIDEO_YELLOW        0xFFFF00

#define CHAR_WIDTH          8
#define CHAR_HEIGHT         16
#define W_SPACING           2
#define H_SPACING           4
#define CHAR_W              (CHAR_WIDTH + W_SPACING)
#define CHAR_H              (CHAR_HEIGHT + H_SPACING)
#define SCREEN_MARGIN       5

extern struct limine_framebuffer *framebuffer;
extern uint32_t *framebuffer_addr;
extern uint32_t screen_width;
extern uint32_t screen_height;
extern uint32_t pitch;

extern uint32_t stride;
extern uint32_t bpp;

extern uint32_t cursor_x;
extern uint32_t cursor_y;

void screen_init();
void screen_putc(char c, uint32_t x, uint32_t y, uint32_t color);
void screen_write(const char *str, uint32_t x, uint32_t y, uint32_t color);
void screen_clear(uint32_t color);
void print_newline(); // 由于某些原因必须保留 for some reasons had to leave

#endif