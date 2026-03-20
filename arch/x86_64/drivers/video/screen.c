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

#include "font.h"
#include "boot/limine.h"
#include "boot/li_req.h"
#include "arch.h"
#include "drivers/video/screen.h"

struct limine_framebuffer *framebuffer = NULL;
uint32_t *framebuffer_addr = NULL;
uint32_t screen_width = 0;
uint32_t screen_height = 0;
uint32_t pitch = 0;
uint32_t stride = 0;
uint32_t bpp = 0;
uint32_t cursor_x = 0;
uint32_t cursor_y = 0;

static void put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (!framebuffer_addr || stride == 0) {
        return;
    }
    if (x >= screen_width || y >= screen_height) {
        return;
    }
    uint64_t offset = (uint64_t)y * (uint64_t)stride + (uint64_t)x;
    uint64_t max_offset = (uint64_t)screen_height * (uint64_t)stride - 1;
    if (offset > max_offset) {
        return;
    }
    framebuffer_addr[offset] = color;
}

void screen_putc(char c, uint32_t x, uint32_t y, uint32_t color) {
    if (c < 32 || c >= 128) {
        c = ' ';
    }
    
    const uint8_t *glyph = vga_font[(uint8_t)c - 32];
    
    for (int row = 0; row < 16; row++) {
        uint8_t byte = glyph[row];
        for (int col = 0; col < 8; col++) {
            if (byte & (1 << (7 - col))) {
                put_pixel(x + col, y + row, color);
            }
        }
    }
}

void screen_init(){
    if (framebuffer_request.response && framebuffer_request.response->framebuffer_count > 0) {
        framebuffer = framebuffer_request.response->framebuffers[0];
        framebuffer_addr = (uint32_t*)framebuffer->address;
        screen_width = framebuffer->width;
        screen_height = framebuffer->height;
        pitch = framebuffer->pitch;
        bpp = framebuffer->bpp;
        stride = framebuffer->pitch / sizeof(uint32_t);
    } else {
        halt();
    }
    screen_clear(VIDEO_BLACK);
    cursor_y = SCREEN_MARGIN;
    cursor_x = SCREEN_MARGIN;
}

void screen_write(const char *str, uint32_t x, uint32_t y, uint32_t color) {
    uint32_t current_x = x;
    while (*str) {
        screen_putc(*str, current_x, y, color);
        current_x += CHAR_W;
        str++;
    }
}

void screen_clear(uint32_t color) {
    if (!framebuffer_addr || stride == 0) return;

    for (uint32_t y = 0; y < screen_height; y++) {
        uint32_t *row = framebuffer_addr + y * stride;
        for (uint32_t x = 0; x < screen_width; x++) {
            row[x] = color;
        }
    }
}

void screen_scroll(uint32_t scroll_px, uint32_t color) {
    if (!framebuffer_addr || stride == 0) return;
    if (scroll_px == 0) return;
    
    uint32_t *fb = (uint32_t *)framebuffer_addr;

    uint32_t pixels_per_line = stride / sizeof(uint32_t);
    
    if (pixels_per_line < screen_width) {
        pixels_per_line = screen_width; 
    }
    
    if (scroll_px >= screen_height) {
        for (uint32_t y = 0; y < screen_height; y++) {
            uint32_t *row = fb + y * pixels_per_line;
            for (uint32_t x = 0; x < screen_width; x++) {
                row[x] = color;
            }
        }
        return;
    }
    
    for (uint32_t dest_row = 0; dest_row < screen_height - scroll_px; dest_row++) {
        uint32_t src_row = dest_row + scroll_px;
        
        uint32_t *dest = fb + dest_row * pixels_per_line;
        uint32_t *src = fb + src_row * pixels_per_line;
        
        for (uint32_t x = 0; x < screen_width; x++) {
            dest[x] = src[x];
        }
    }
    
    for (uint32_t row = screen_height - scroll_px; row < screen_height; row++) {
        uint32_t *row_ptr = fb + row * pixels_per_line;
        for (uint32_t x = 0; x < screen_width; x++) {
            row_ptr[x] = color;
        }
    }
}

void print_newline() {
    cursor_y += CHAR_H;

    if (cursor_y > screen_height - SCREEN_MARGIN) {
        screen_scroll(CHAR_H, VIDEO_BLACK);
        cursor_y -= CHAR_H;
    }
}